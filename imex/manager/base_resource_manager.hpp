//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MANAGER_BASE_RESOURCE_MANAGER_HPP_
#define _SNAKEOIL_IMEX_MANAGER_BASE_RESOURCE_MANAGER_HPP_

#include "../typedefs.h"
#include "../result.h"
#include "../protos.h"

#include <snakeoil/resource/manager/resource_manager.hpp>
#include <snakeoil/resource/handle/handle.hpp>

namespace so_imex
{
    template< typename T >
    class base_resource_manager
    {
        so_this_typedefs( base_resource_manager< T > ) ;
        so_typedefs( T, type ) ;

    protected:

        struct manage_item
        {
            type_ptr_t data_ptr = nullptr ;
            so_io::path_t file_path ;
        };
        so_typedef( manage_item ) ;

        so_typedefs( so_resource::resource_manager<manage_item_t>, resource_manager ) ;
        resource_manager_t _res_mgr ;

    public:

        so_typedefs( typename resource_manager_t::handle_t, handle ) ;        

        so_typedefs( typename resource_manager_t::handles_t, handles ) ;

    public:

        struct manage_params
        {
            type_ptr_t data_ptr = nullptr ;
            so_io::path_t file_path ;
        };
        so_typedef( manage_params ) ;

    private:

        so_thread::mutex_t _mtx_counter ;
        size_t _counter = 0 ;

    private:  /// copy ctor

        base_resource_manager( this_cref_t ) {}

    protected:

        base_resource_manager( void_t ) {}
        base_resource_manager( this_rref_t rhv ) 
        {
            _counter = rhv._counter ;
            _res_mgr = std::move( rhv._res_mgr ) ;
        }

    public:

        virtual ~base_resource_manager( void_t ) {}

    protected:

        void_t do_destruction( typename resource_manager_t::for_each_funk_t funk )
        {
            _res_mgr.call_for_each( funk ) ;
            _res_mgr.call_for_each( [&]( manage_item_ptr_t item_ptr )
            {
                so_imex::memory::dealloc( item_ptr ) ;
            } ) ;
        }


    public: // acquire/release

        bool_t acquire_by_path( so_io::path_cref_t path_in, 
            so_resource::purpose_cref_t p, handle_ref_t hnd_out )
        {
            return _res_mgr.acquire_if( [&]( manage_item_ptr_t item_ptr )
            {
                return item_ptr->file_path == path_in ;
            }, p, hnd_out ) ;
        }

        bool_t acquire( so_imex::key_cref_t key_in, 
            so_resource::purpose_cref_t p, handle_ref_t hnd_out ) 
        {
            return _res_mgr.acquire( key_in, p, hnd_out ) ;
        }

        bool_t release( handle_rref_t hnd ) 
        {
            return _res_mgr.release( std::move( hnd ) ) ;
        }

        void_t acquire_all( so_resource::purpose_cref_t p, handles_out_t handles )
        {
            _res_mgr.acquire_all( p, handles ) ;
        }

    public: // insert/reserve/exchange

        /// does not acquire the resource. It only inserts it.
        so_imex::result insert( so_imex::key_cref_t key_in, manage_params_cref_t mp ) 
        {
            manage_item mi;
            mi.data_ptr = mp.data_ptr ;
            mi.file_path = mp.file_path ;

            manage_item_ptr_t mi_ptr = so_imex::memory::alloc( std::move( mi ),
                "[so_imex::base_resource_manager::manage] : manage item" ) ;

            auto const res = _res_mgr.insert( key_in, mi_ptr ) ;
            if( so_log::global::error( so_resource::no_success( res ),
                "[so_imex::base_resource_manager::manage] : insert_and_acquire" ) )
            {
                so_imex::memory::dealloc( mi_ptr ) ;
                return so_imex::already_managed ;
            }

            return so_imex::ok ;
        }

        so_imex::result reserve_by_key( so_imex::key_cref_t key_in ) 
        {
            auto const res = _res_mgr.reserve( key_in ) ;

            if( res == so_resource::reserved )
            {
                manage_item mi;
                mi.data_ptr = nullptr ;
                mi.file_path = "" ;

                manage_item_ptr_t mi_ptr = so_imex::memory::alloc( std::move( mi ),
                    "[so_imex::base_resource_manager::manage] : manage item" ) ;

                manage_item_ptr_t mi_old ;
                auto const lres = _res_mgr.exchange( key_in, mi_ptr, mi_old ) ;
                so_log::global::error_and_exit( so_core::is_not( lres ),
                    "[so_imex::base_resource_manager::reserve_by_path] : exchange must be successful" ) ;

                return so_imex::reserved ;
            }
            else if( res == so_resource::found )
                return so_imex::found ;

            return so_imex::failed ;
        }

        so_imex::result reserve_by_path( so_io::path_cref_t by_path, so_imex::key_ref_t key_inout ) 
        {
            auto const res = _res_mgr.reserve_or_find_if( [&]( manage_item_ptr_t item_ptr )
            {
                return item_ptr->file_path == by_path ;
            }, key_inout ) ;

            // if the resource could be reserved, an empty 
            // data carrier is placed.
            if( res == so_resource::reserved )
            {
                manage_item mi;
                mi.data_ptr = nullptr ;
                mi.file_path = by_path ;

                manage_item_ptr_t mi_ptr = so_imex::memory::alloc( std::move( mi ),
                    "[so_imex::base_resource_manager::manage] : manage item" ) ;

                manage_item_ptr_t mi_old ;
                auto const lres = _res_mgr.exchange( key_inout, mi_ptr, mi_old ) ;
                so_log::global::error_and_exit( so_core::is_not( lres ),
                    "[so_imex::base_resource_manager::reserve_by_path] : exchange must be successful" ) ;

                return so_imex::reserved ;
            }
            else if( res == so_resource::found ) return so_imex::found ;

            so_log::global::error( "[so_imex::base_resource_manager::reserve_by_path] : reserve_or_find_if" ) ;
            return so_imex::invalid ;
        }

        /// only exchanged the resource. It does not acquire or release any resource.
        /// the exchanged image resource will be destroyed
        so_imex::result exchange( so_imex::key_cref_t key_in, manage_params_cref_t mp ) 
        {
            auto const res = _res_mgr.exchange( key_in, [&]( manage_item_ptr_t mi )
            {
                mi->file_path = mp.file_path ;
                mi->data_ptr = mp.data_ptr ;
                return true ;
            } ) ;

            if( so_log::global::error( so_core::is_not( res ), 
                "[so_imex::base_resource_manager::exchange]" ) )
                return so_imex::failed ;

            return so_imex::ok ;
        }

    public:


        /// take all the data from the other manager and
        /// place it into this manager.
        so_imex::result take_and_clear( this_ref_t other ) 
        {
            _res_mgr.take_all_items_from( other._res_mgr ) ;
            return so_imex::ok ;
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
