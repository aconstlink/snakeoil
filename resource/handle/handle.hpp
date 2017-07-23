//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_RESOURCE_HANDLE_HANDLE_H_
#define _SNAKEOIL_RESOURCE_HANDLE_HANDLE_H_

#include "../typedefs.h"
#include "../result.h"
#include "../api.h"
#include "../protos.h"

#include "../manager/manager_listener.hpp"

#include <snakeoil/core/macros/move.h>

namespace so_resource
{
    /// unique handle - non copyable
    template< typename T >
    class handle
    {
        so_this_typedefs( handle<T> ) ;
        so_typedefs( so_resource::resource_manager<T>, manager ) ;

        so_typedefs( T, type ) ;
    
        so_typedefs( this_t, handle ) ;

    private:

        so_typedefs( manager_listener<T>, manager_listener ) ;
        friend class manager_listener_t ;

        manager_listener_ptr_t _manager_listener_ptr = nullptr ;

    private:

        friend manager_t ;

    private: // handle data

        so_resource::key_t _key ;

        /// fast access
        /// we store the pointer to the data. It is
        /// safe to do that because the resource is
        /// ref counted in the manager and will 
        /// pertain its address.
        type_ptr_t _ptr = nullptr ;

        /// id to the purpose of acquisition
        size_t _pid = size_t(-1) ;

        /// the resource manager that stores the data
        manager_ptr_t _manager_ptr = nullptr ;

    private: // local data

        /// ref count of local instances. This ref count does not
        /// represent the number of instances of the handle itself.
        /// The handle's ref count is stored in the manager.
        size_t _local_ref_count = 0 ;

    private:

        handle( this_cref_t )
        {
            so_core::break_if( true ) ;
        }

        this_rref_t operator = ( this_cref_t )
        {
            so_core::break_if( true ) ;
            return *this ;
        }

    private: // resource manager interfaces

        /// allows only the resource manager to instantiate acquired manager resources.
        handle( so_resource::key_cref_t key, type_ptr_t ptr, size_t pid, manager_ptr_t mptr ) : 
            _key(key), _ptr(ptr), _pid(pid), _manager_ptr(mptr) 
        {
            manager_listener_t mgr_listener(this, [=]( handle_ptr_t owner, manager_ptr_t ptr )
            {
                owner->_manager_ptr = ptr ;
            } ) ;

            _manager_listener_ptr = so_resource::memory::alloc( std::move(mgr_listener), 
                "[so_imex::handle::handle] : manager_listener : " + key ) ;
        }
        
        size_t get_pid( void_t ) const
        {
            return _pid ;
        }

        /// the idea of this function is to allow the resource manager to 
        /// clear the handle if the handle was released via the resource manager.
        /// this is important, because the handle releases itself at dtor time
        /// at its resource manager
        void_t invalidate( void_t )
        {
            so_log::log::warning( _local_ref_count != 0, 
                "[so_resource::handle::invalidate] : not all local references released" ) ;

            _key = so_resource::key_t() ;
            _ptr = nullptr ;
            _pid = size_t(-1) ;
            
            _manager_ptr = nullptr ;
        }

    public:

        handle( void_t ) 
        {}

        handle( this_rref_t rhv )
        {
            *this = std::move(rhv) ;
        }

        ~handle( void_t )
        {
            so_log::log::warning( _local_ref_count != 0, 
                "[so_resource::handle::~handle] : not all local references released" ) ;

            if( so_core::is_not_nullptr(_manager_ptr) )
                _manager_ptr->release( std::move(*this) ) ;

            // must be done after the release
            if( so_core::is_not_nullptr(_manager_listener_ptr) )
                so_resource::memory::dealloc(_manager_listener_ptr) ;
        }

    public:

        this_ref_t operator = ( this_rref_t rhv )
        {
            _key = std::move( rhv._key ) ;

            so_move_member_ptr( _ptr, rhv ) ;
            so_move_member_ptr( _manager_ptr, rhv ) ;
            so_move_member_ptr( _manager_listener_ptr, rhv ) ;

            if( so_core::is_not_nullptr(_manager_listener_ptr) )
                _manager_listener_ptr->change_owner( this ) ;

            _local_ref_count = rhv._local_ref_count ;
            rhv._local_ref_count = 0 ;

            _pid = rhv._pid ;
            rhv._pid = size_t( -1 ) ;

            return *this ;
        }

        type_ptr_t operator -> ( void_t ) 
        {
            so_log::log::error_and_exit( so_core::is_nullptr(_ptr), 
                "[so_resource::handle::operator ->()] : fast access ptr == nullptr" ) ;

            return _ptr ;
        }

        type_cptr_t operator -> ( void_t ) const
        {
            so_log::log::error_and_exit( so_core::is_nullptr( _ptr ),
                "[so_resource::handle::operator ->()] : fast access ptr == nullptr" ) ;

            return _ptr ;
        }

    public:

        type_ptr_t acquire_local( void_t )
        {
            ++_local_ref_count ;
            return _ptr ;
        }

        void_t release_local( void_t ) 
        {
            so_log::log::error_and_exit( _local_ref_count == 0,
                "[so_resource::handle::release_local] : _ref_count" ) ;

            --_local_ref_count ;
        }

        so_resource::key_cref_t get_key( void_t ) const
        {
            return _key ;
        }

    public:

        bool_t is_valid( void_t ) const 
        {
            return so_core::is_not_nullptr(_manager_ptr) ;
        }


    };
}

#endif
