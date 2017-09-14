//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_RESOURCE_MANAGER_RESOURCE_MANAGER_HPP_
#define _SNAKEOIL_RESOURCE_MANAGER_RESOURCE_MANAGER_HPP_

#include "../typedefs.h"
#include "../api.h"
#include "../result.h"
#include "../protos.h"

#include "manager_listener.hpp"

#include <snakeoil/std/container/map.hpp>
#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/std/container/list.hpp>
#include <snakeoil/std/container/stack.hpp>

#include <snakeoil/thread/mutex.h>
#include <snakeoil/log/global.h>

namespace so_resource
{
    //***********************************************************************
    template< typename T >
    class resource_manager
    {
        so_this_typedefs( resource_manager<T> );
        so_typedefs( T, type ) ;

    public:

        so_typedefs( so_resource::handle<T>, handle ) ;
        so_typedefs( so_std::vector<handle_t>, handles ) ;

    public:

        typedef std::function< void_t( type_ptr_t ) > for_each_funk_t ;
        typedef std::function< bool_t( type_ptr_t) > find_funk_t ;
        typedef std::function< bool_t( type_ptr_t) > exchange_funk_t ;
        typedef std::function< bool_t( type_ptr_t) > release_funk_t ;

    private:

        so_typedefs( so_std::vector< so_std::string_t >, purposes ) ;

        typedef so_std::stack< size_t, 5 > _stack_t ;
        so_typedefs( _stack_t, indices ) ;

        so_typedefs( manager_listener< T >, manager_listener ) ;
        so_typedefs( so_std::vector< manager_listener_ptr_t >, manager_listeners ) ;

    private:

        //********************************************************
        struct purpose_item
        {
            so_this_typedefs( purpose_item ) ;

            purposes_t purposes ;
            indices_t free_indices ;

        public:

            purpose_item( void_t ){}
            purpose_item( this_rref_t rhv )
            {
                *this = std::move(rhv) ;
            }
            ~purpose_item( void_t ) {}

            this_ref_t operator = ( this_rref_t rhv )
            {
                purposes = std::move( rhv.purposes ) ;
                free_indices = std::move( rhv.free_indices ) ;
                return *this ;
            }      

            purpose_item( this_cref_t ) {}
        };
        so_typedef( purpose_item ) ;

        //********************************************************
        struct store_item
        {
            so_typedef( store_item ) ;

            type_ptr_t user_data = nullptr ;
            size_t ref_count = 0 ;
            
            /// per acquirement, a purpose is required.
            /// all purposes for this item is stored here.
            purpose_item_t purposes ;

            manager_listeners_t listeners ;

        public:

            store_item( void_t ){}
            store_item( this_cref_t ) = delete ;
            store_item( store_item && rhv )
            {
                *this = std::move(rhv) ;
            }
            ~store_item( void_t ) {}

            this_ref_t operator = ( this_cref_t ) = delete ;
            store_item & operator = ( store_item && rhv )
            {
                so_move_member_ptr( user_data, rhv ) ;
                ref_count = rhv.ref_count ;
                purposes = std::move( rhv.purposes ) ;
                listeners = std::move( rhv.listeners ) ;

                return *this ;
            }
        };
        so_typedef( store_item ) ;

    private:

        typedef so_std::map< so_resource::key_t, typename this_t::store_item_t > __key_to_item_t ;
        so_typedefs( __key_to_item_t, key_to_item ) ;

        key_to_item_t _items ;

        so_thread::mutex_t _mtx ;

    private:

        size_t acquire_purpose_id( store_item_ref_t si, so_resource::purpose_cref_t p )
        {
            if( si.purposes.free_indices.has_item() )
            {
                size_t const pid = si.purposes.free_indices.pop() ;
                si.purposes.purposes[pid] = p ;
                return pid ;
            }

            if( si.purposes.purposes.size() == si.purposes.purposes.capacity() )
            {
                si.purposes.purposes.reserve( si.purposes.purposes.size() + 10 ) ;
            }

            size_t const pid = si.purposes.purposes.size() ;
            si.purposes.purposes.push_back( p ) ;

            return pid ; ;
        }

        void_t release_purpose_id( store_item_ref_t si, size_t id )
        {
            si.purposes.free_indices.push( id ) ;
            si.purposes.purposes[id] = so_resource::purpose_t() ;
        }

    private:

        resource_manager( this_cref_t ){}

    public:

        resource_manager( void_t ) 
        {
        }

        resource_manager( this_rref_t rhv ) 
        {
            *this = std::move(rhv) ;
        }

        ~resource_manager( void_t ) 
        {
            /*for( auto const & item : _items )
            {
                so_log::global::warning( item.second.ref_count != 0, 
                    "[so_resource::~resource_manager] : ref count not 0" ) ;
            }*/
        }

    public:

        this_ref_t operator = ( this_rref_t rhv )
        {
            _items = std::move( rhv._items ) ;
            return *this ;
        }

        this_ref_t call_for_each( for_each_funk_t funk ) 
        {
            so_thread::lock_guard_t lk(_mtx) ;

            for( auto & item : _items )
            {
                funk( item.second.user_data ) ;
            }
            return *this ;
        }

    public: // insert // erase

        /// allows to insert an item only if the key is not used.
        so_resource::result insert( so_resource::key_cref_t key_in, type_ptr_t data )
        {
            so_thread::lock_guard_t lk(_mtx) ;

            auto iter = _items.find( key_in ) ;
            if( iter != _items.end() )
                return so_resource::already_managed ;

            store_item si ;
            si.ref_count = 0 ;
            si.user_data = data ;

            _items[key_in] = std::move(si) ;
            
            return so_resource::ok ;
        }

        /// items with a ref_count > 0 can not be erased.
        /// all acquired handles must be released first.
        so_resource::result erase( so_resource::key_cref_t key_in, type_ptr_t & data_out )
        {
            so_thread::lock_guard_t lk(_mtx) ;

            auto iter = _items.find( key_in ) ;
            if( iter == _items.end() )
                return so_resource::invalid_argument ;

            if( iter->second.ref_count != 0 )
                return so_resource::invalid_refcount ;

            data_out = iter->second.user_data ;
            iter->second.user_data = nullptr ;

            _items.erase( iter ) ;
            
            return so_resource::ok ;
        }

    public: // acquire

        /// @return false if key does not exist
        /// @return true otherwise
        bool_t acquire( so_resource::key_cref_t key_in, 
            so_resource::purpose_cref_t p, handle_ref_t hnd_out )
        {            
            so_thread::lock_guard_t lk(_mtx) ;

            if( so_log::global::error(hnd_out.is_valid(), 
                "[so_resource::resource_manager::acquire] : valid handle" ) )
                return false ;

            auto iter = _items.find( key_in ) ;
            if( iter == _items.end() )
                return false ;

            store_item_ref_t si = iter->second ;
            return this_t::_acquire_( si, key_in, p, hnd_out ) ;
        }

        bool_t acquire_if( find_funk_t funk,
            so_resource::purpose_cref_t p, handle_ref_t hnd_out )
        {
            if(so_log::global::error( hnd_out.is_valid(),
                "[so_resource::resource_manager::acquire] : valid handle" ))
                return false ;

            so_thread::lock_guard_t lk(_mtx) ;

            for( auto & item : _items )
            {
                if( funk( item.second.user_data ) )
                {
                    return this_t::_acquire_( item.second, item.first, p, hnd_out ) ;
                }
            }
            return false ;
        }

        /// acquire all resource within this manager
        /// @note slow and locking operation
        void_t acquire_all( so_resource::purpose_cref_t p, handles_out_t handles )
        {
            so_thread::lock_guard_t lk( _mtx ) ;

            handles.resize( _items.size() ) ;

            size_t i = 0 ;
            for( auto & item : _items )
            {
                this_t::_acquire_( item.second, item.first, p, handles[i++] )  ;
            }
        }

    private:

        bool_t _acquire_( store_item_ref_t si, so_resource::key_cref_t key_in,
            so_resource::purpose_cref_t p, handle_ref_t hnd_out )
        {
            si.ref_count++ ;

            size_t const pid = this_t::acquire_purpose_id( si, p ) ;

            hnd_out = handle_t( key_in, si.user_data, pid, this ) ;
            this_t::add_listener( hnd_out, si ) ;

            return true ;
        }

        void_t add_listener( handle_ref_t hnd, store_item_ref_t si )
        {
            si.listeners.push_back( hnd._manager_listener_ptr ) ;
        }

    public: // release

        bool_t release( handle_inout_t hnd, release_funk_t funk )
        {
            so_thread::lock_guard_t lk( _mtx ) ;

            handle_t int_handle = std::move( hnd ) ;

            auto iter = _items.find( int_handle.get_key() ) ;
            so_log::global::error_and_exit( iter == _items.end(),
                "[so_resource::resource_manager::release] : handle must be valid" ) ;

            store_item_ref_t si = iter->second ;

            so_log::global::error_and_exit( si.ref_count == 0,
                "[so_resource::resource_manager::release] : must not ref_count == 0" ) ;

            this_t::release_purpose_id( si, int_handle.get_pid() ) ;

            {
                auto iiter = std::find( si.listeners.begin(), si.listeners.end(),
                    int_handle._manager_listener_ptr ) ;

                if( so_core::is_not( so_log::global::error( iiter == si.listeners.end(),
                    "[so_resouce::resource_manager::release] : listener not found" ) ) )
                {
                    si.listeners.erase( iiter ) ;
                }
            }

            int_handle.invalidate() ;

            si.ref_count-- ;

            if( si.ref_count == 0 )
            {
                if( funk( si.user_data ) )
                {
                    _items.erase( iter ) ;
                }
                return true ;
            }
            return false ;
        }

        /// @return true if ref count == 0 and calls funk
        /// if funk returns true, the resource entry will be 
        /// erased completely. 
        /// @remarks if funk returns true, be sure to properly handle 
        /// the user_data. Proper handling could be remembering the 
        /// user_data pointer elsewhere or just destroying the user data
        /// within funk.
        bool_t release( handle_rref_t hnd, release_funk_t funk )
        {
            return this_t::release( hnd, funk ) ;
        }

        /// @return true if ref count == 0
        /// @return false otherwise
        /// @remark if this function returns true, there are no
        /// instances that use this resource. The external entity can
        /// decide what to do with the resource. But be aware, that it is
        /// still possible that some other entity can acquire the 
        /// resource in the meantime.
        bool_t release( handle_rref_t hnd )
        {           
            return this_t::release( std::move(hnd), 
                [=]( this_t::type_ptr_t ){ return false ; } ) ;
        }

        

    public: // find

        bool_t find_key_if( find_funk_t funk, so_resource::key_t key_out )
        {
            so_thread::lock_guard_t lk(_mtx) ;

            bool_t found = false ;
            for( auto & item : _items )
            {
                if( funk( item.second.user_data) )
                {
                    key_out = item.first ;
                    found = true ;
                    break ;
                }
            }
            return found ;
        }

        /// @todo needs more testing

        this_ref_t take_all_items_from( this_ref_t other )
        {              
            so_thread::lock_guard_t lk_a(other._mtx) ;

            {
                bool_t can_move = true ;
                for( auto const & item : other._items )
                {
                    for( auto * lptr : item.second.listeners )
                    {
                        lptr->on_manager_changed( this ) ;
                    }

                    /*if( item.second.ref_count )
                    {
                        can_move = false ;
                        break ;
                    }*/
                }
                if( so_core::is_not(can_move) )
                {
                    so_log::global::error( 
                        "[so_resource::resource_manager::take_all_items_from] : \
                        ref counts not zero" ) ;
                    return *this ;
                }
            }

            so_thread::lock_guard_t lk_b(_mtx) ;

            for( auto & item : other._items )
            {
                auto iter = _items.find( item.first ) ;
                
                if( so_log::global::warning( iter != _items.end(), 
                    "[so_resource::resource_manager::take_all_items_from] : insert"  )  ) 
                    continue ;

                _items[item.first] = std::move(item.second) ;
            }

            other._items.clear() ;
            
            return *this ;
        }

        
    public:

        so_resource::result reserve( key_cref_t key_in )
        {
            so_thread::lock_guard_t lk( _mtx ) ;

            auto iter = _items.find( key_in ) ;
            if( iter == _items.end() )
            {            
                this_t::store_item si ;
                si.ref_count = 0 ;
                si.user_data = nullptr ;
                
                _items[key_in] = std::move(si) ;
                                
                return so_resource::reserved ;
            }             

            return so_resource::found ;
        }

        /// allows to reserve a store item without actually passing
        /// the item into the resource manager. This function is useful
        /// when the item to be stored is not fully known but the 
        /// entry must be reserved for later usage. the function also
        /// acquires the resource.
        so_resource::result reserve_and_acquire( key_cref_t key_in, 
            so_resource::purpose_cref_t p, handle_out_t hnd_out )
        {            
            so_thread::lock_guard_t lk( _mtx ) ;

            auto iter = _items.find( key_in ) ;
            if( iter == _items.end() )
            {
                store_item si ;
                si.ref_count = 0 ;
                si.user_data = nullptr ;

                _acquire_( si, key_in, p, hnd_out ) ;

                _items[key_in] = std::move(si) ;

                return so_resource::reserved ;
            }
            
            _acquire_( iter->second, key_in, p, hnd_out ) ;
            
            return so_resource::found ;
        }

        so_resource::result reserve_or_find_if( find_funk_t find_if_funk, key_ref_t key_inout )
        {
            
            so_thread::lock_guard_t lk(_mtx) ;

            // Step 1: check based on the find funk. If found,
            // the resource already exists.
            bool_t found = false ;
            for( auto & item : _items )
            {
                if( find_if_funk( item.second.user_data ) )
                {
                    found = true ;

                    key_inout = item.first ;

                    break ;
                }
            }

            if( found == true )
                return so_resource::found ;

            // Step 2: If the resource is not found by funk, find the resource
            // based on the passed key_inout. If that resource is found, 
            // this operation is invalid.
            auto iter = _items.find( key_inout ) ;
            if( iter != _items.end() )
                return so_resource::invalid ;

            // Step 3: If the resource is still not found, reserve it.
            store_item si ;
            si.ref_count = 0 ;
            si.user_data = nullptr ;

            _items[key_inout] = std::move(si) ;
            
            return so_resource::reserved ;
        }
       
        so_resource::result insert_or_exchange( key_cref_t key_in,
            type_ptr_t data, type_ptr_t & old_data )
        {
            
            so_thread::lock_guard_t lk( _mtx ) ;

            auto iter = _items.find( key_in );

            // insert
            if( iter == _items.end() )
            {
                store_item si ;
                si.ref_count = 0 ;
                si.user_data = data ;

                _items[key_in] = si ;


                old_data = nullptr ;

                return so_resource::inserted ;
            }
            // exchange
            else
            {
                old_data = iter->second.user_data ;
                iter->second.user_data = data ;

                return so_resource::exchanged ;
            }
            
            return so_resource::invalid ;
        }

        /// allows to exchange the data carrier. 
        /// @note if the ref count of that user data is
        /// not 0, the function will fail. Try the exchange 
        /// function that allows to exchange data only.
        bool_t exchange( so_resource::key_cref_t key_in,
            type_ptr_t new_data, type_ptr_t & old_data )
        {
            so_thread::lock_guard_t lk( _mtx ) ;

            auto iter = _items.find( key_in ) ;
            if( iter == _items.end() )
                return false ;

            // at the moment, it is not possible to
            // exchange data that has ref counts.
            // this is due to handles using the data 
            // carrier pointer.
            // @todo introduce listeners/events
            if( iter->second.ref_count != 0 &&
                so_core::is_not_nullptr(iter->second.user_data) )
                return false ;
            
            old_data = iter->second.user_data ;
            iter->second.user_data = new_data ;
            
            return true ;
        }

        /// allows to exchange data if the user knows that
        /// only one instance is holding the data, i.e. the
        /// callee must hold the handle.
        bool_t exchange_self( handle_ref_t hnd, 
            type_ptr_t new_data, type_ptr_t & old_data )
        {
            so_thread::lock_guard_t lk( _mtx ) ;            

            auto iter = _items.find( hnd.get_key() ) ;
            so_log::global::error_and_exit( iter == _items.end(),
                "[so_resource::resource_manager::exchange_self] : handle must be valid" ) ;

            // if the user exchanges data for a handle by
            // this function, there must be no other entity
            // that holds the resource.
            // @todo introduce listeners/events
            if( iter->second.ref_count > 1 )
                return false ;

            old_data = iter->second.user_data ;
            iter->second.user_data = new_data ;

            return true ;
        }

        /// allows the user to exchange the data only by 
        /// leaving the data carrier struct untouched.
        bool_t exchange( so_resource::key_cref_t key_in, 
            exchange_funk_t funk )
        {
            so_thread::lock_guard_t lk( _mtx ) ;

            auto iter = _items.find( key_in ) ;
            if( iter == _items.end() )
                return false ;

            return funk( iter->second.user_data ) ;
        }

        //
        // exchanging user data requires all acquired handles to be informed
        //
        
        /*
        /// only exchanges if funk is true!
        bool_t insert_or_exchange_if( find_funk_t exchange_if_funk,
        key_ref_t key_inout, type_ptr_t data, type_ptr_t & data_out )
        {
        so_thread::lock_guard_t lk( _mtx ) ;

        // Step 1: check based on the find funk. If found,
        // exchange the resource.
        bool_t found = false ;
        for( auto & item : _items )
        {
        if( exchange_if_funk( item.second.user_data ) )
        {
        found = true ;

        data_out = item.second.user_data ;
        item.second.user_data = data ;
        key_inout = item.first ;

        break ;
        }
        }

        if( found == true )
        return true ;

        // Step 2: If the resource is not found by funk, find the resource
        // based on the passed key_inout. If that resource is found,
        // this operation is invalid.
        auto iter = _items.find( key_inout ) ;
        if( iter != _items.end() )
        return false ;

        // Step 3: If the resource is still not found, just insert it.
        store_item si ;
        si.ref_count = 0 ;
        si.user_data = data ;

        _items[key_inout] = si ;

        return true ;
        }

        

    public:

        so_resource::result erase( so_resource::key_cref_t key_in )
        {
            auto iter = _items.find( key_in ) ;
            if( iter == _items.end() )
                return so_resource::invalid_argument ;

            if( iter->second.ref_count != 0 )
                return so_resource::ref_count_not_null ;

            _items.erase( iter ) ;

            return so_resource::ok ;
        }
        */

    };
}

#endif
