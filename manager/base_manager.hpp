//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_BASE_MANAGER_HPP_
#define _SNAKEOIL_MANAGER_BASE_MANAGER_HPP_

#include "typedefs.h"
#include "protos.h"
#include "api.h"
#include "result.h"

#include <snakeoil/resource/handle/handle.hpp>
#include <snakeoil/resource/manager/resource_manager.hpp>

namespace so_manager
{
    template< typename T >
    class base_manager
    {
        so_this_typedefs( base_manager<T> ) ;
        so_typedefs( T, type ) ;

    protected:

        struct store_item
        {
            so_this_typedefs( store_item ) ;

            type_ptr_t data_ptr = nullptr ;
            

            store_item( void_t ) {}
            store_item( this_cref_t ) = delete ;
            store_item( this_rref_t rhv ) 
            {
                so_move_member_ptr( data_ptr, rhv ) ;
            }
            this_ref_t operator = ( this_cref_t ) = delete ;
            this_ref_t operator = ( this_rref_t rhv )
            {
                so_move_member_ptr( data_ptr, rhv ) ;
                return *this ;
            }
        };
        so_typedef( store_item ) ;
        
        so_typedefs( so_resource::resource_manager<store_item_t>, resource_manager ) ;
        resource_manager_t _res_mgr ;

    public:

        so_typedefs( typename resource_manager_t::handle_t, handle ) ;
        so_typedefs( typename resource_manager_t::handles_t, handles ) ;


        struct manage_params
        {
            type_ptr_t data_ptr = nullptr;
        };
        so_typedef( manage_params ) ;

    private:

        so_thread::mutex_t _mtx_counter ;
        size_t _counter = 0 ;

    public:

        base_manager( void_t ) 
        {
        }

        base_manager( this_cref_t ) = delete ;
        base_manager( this_rref_t rhv ) : _counter( rhv._counter ), 
            _res_mgr( std::move( rhv._res_mgr ) )
        {}

        this_ref_t operator = ( this_cref_t ) = delete ;
        this_ref_t operator = ( this_rref_t rhv )
        {
            _counter = rhv._counter ;
            _res_mgr = std::move( rhv._res_mgr ) ;
            return *this ;
        }

        virtual ~base_manager( void_t ) 
        {   
        }

    protected:

        void_t do_destruction( typename resource_manager_t::for_each_funk_t funk )
        {
            _res_mgr.call_for_each( funk ) ;
        }

    public: // acquire/release

        bool_t acquire( so_manager::key_cref_t key_in,
            so_resource::purpose_cref_t p, handle_out_t hnd_out )
        {
            return _res_mgr.acquire( key_in, p, hnd_out ) ;
        }

        void_t acquire_all( so_resource::purpose_cref_t p, handles_out_t handles )
        {
            _res_mgr.acquire_all( p, handles ) ;
        }

        bool_t release( handle_rref_t hnd )
        {
            return _res_mgr.release( std::move( hnd ) ) ;
        }

    public: // insert/reserve/exchange

        /// does not acquire the resource. It only inserts it.
        so_manager::result insert( so_manager::key_cref_t key_in, manage_params_cref_t mp )
        {
            store_item si;
            si.data_ptr = mp.data_ptr ;

            store_item_ptr_t si_ptr = so_manager::memory::alloc( std::move( si ),
                "[so_manager::base_manager::insert] : store_item" ) ;

            auto const res = _res_mgr.insert( key_in, si_ptr ) ;
            if( so_log::log::error( so_resource::no_success( res ),
                "[so_manager::base_manager::insert] : insert" ) )
            {
                so_manager::memory::dealloc( si_ptr ) ;
                return so_manager::key_already_in_use;
            }

            return so_manager::ok ;
        }

        /// reserves the resource by key so that no other entity can get the name
        so_manager::result reserve_by_key( so_manager::key_cref_t key_in )
        {
            auto const res = _res_mgr.reserve( key_in ) ;

            if( res == so_resource::reserved )
            {
                store_item si;
                si.data_ptr = nullptr ;

                store_item_ptr_t si_ptr = so_manager::memory::alloc( std::move( si ),
                    "[so_manager::base_manager::reserve_by_key] : store_item" ) ;

                store_item_ptr_t si_old ;
                auto const inner_res = _res_mgr.exchange( key_in, si_ptr, si_old ) ;
                so_log::log::error_and_exit( so_core::is_not( inner_res ),
                    "[so_manager::base_manager::reserve_by_key] : exchange must be successful" ) ;

                return so_manager::reserved ;
            }
            else if( res == so_resource::found )
                return so_manager::found ;

            return so_manager::failed ;
        }

        /// only exchanges the resource. It does not acquire or release any resource.
        /// the exchanged resource will be destroyed
        so_manager::result exchange( so_manager::key_cref_t key_in, manage_params_cref_t mp )
        {
            auto const res = _res_mgr.exchange( key_in, [&]( store_item_ptr_t si )
            {
                si->data_ptr = mp.data_ptr ;
                return true ;
            } ) ;

            if( so_log::log::error( so_core::is_not( res ),
                "[so_manager::base_manager::exchange]" ) )
                return so_manager::failed ;

            return so_manager::ok ;
        }

    public:

        size_t unique_number( void_t )
        {
            so_thread::lock_guard_t lk( _mtx_counter ) ;
            return _counter++ ;
        }

        so_std::string_t unique_number_as_string( void_t )
        {
            size_t const number = this_t::unique_number() ;
            return so_std::string_t( std::to_string( number ) ) ;
        }

    public:

        virtual void_t destroy( void_t ) = 0 ;

    };
}

#endif