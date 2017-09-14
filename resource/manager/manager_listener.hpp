//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_RESOURCE_MANAGER_MANAGER_LISTENER_HPP_
#define _SNAKEOIL_RESOURCE_MANAGER_MANAGER_LISTENER_HPP_

#include "../protos.h"

#include <snakeoil/core/macros/move.h>

#include <functional>

namespace so_resource
{
    template< typename T >
    class manager_listener
    {
        so_this_typedefs( manager_listener<T> ) ;

    protected:

        so_typedefs( T, type ) ;
        so_typedefs( so_resource::resource_manager<T>, manager ) ;

        typedef so_resource::handle< T > * handle_ptr_t ;

    public:

        typedef std::function< void_t( handle_ptr_t, manager_ptr_t ) > manager_changed_funk_t ;

    private:

        handle_ptr_t _owner = nullptr ;
        manager_changed_funk_t _manager_changed ;

    public:

        manager_listener( handle_ptr_t owner, manager_changed_funk_t mgr_funk ) : 
            _owner(owner), _manager_changed(mgr_funk)
        {}

        manager_listener( this_rref_t rhv )
        {
            so_move_member_ptr( _owner, rhv ) ;
            _manager_changed = std::move(rhv._manager_changed) ;
        }

        ~manager_listener( void_t ) {}

    public:

        void_t change_owner( handle_ptr_t owner )
        {
            _owner = owner ;
        }

    public:

        void_t on_manager_changed( manager_ptr_t ptr )
        {
            _manager_changed( _owner, ptr ) ;
        }
    };
}

#endif
