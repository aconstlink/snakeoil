//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "input_slot.h"
#include "../ioutput_slot.h"

#include <snakeoil/thread/semaphore_guard.hpp>
#include <snakeoil/log/log.h>

using namespace so_flow ;

//***********************************************************************
input_slot::input_slot( void_t )
{}

//***********************************************************************
input_slot::input_slot( this_rref_t rhv )
{
    so_log::log::error_and_exit( so_core::is_not_nullptr( _os_ptr ),
        "[input_slot] : can not move if output slot set" ) ;

    _in_connect = std::move( rhv._in_connect ) ;
    
    so_move_member_ptr( _owner_ptr, rhv ) ;
}

//***********************************************************************
input_slot::~input_slot( void_t )
{
    this_t::disconnect() ;
}

//***********************************************************************
so_flow::result input_slot::connect( so_flow::ioutput_slot_ptr_t os_ptr )
{
    // check os pointers
    // also handles reentrance
    if( _os_ptr == os_ptr )
        return so_flow::already_connected ;

    so_thread::semaphore_guard_t d( &_in_connect ) ;
    if( _in_connect == 2 )
    {
        so_log::log::warning( "[so_flow::input_slot::connect] : \
                               can not enter input slot connect more than once" ) ;
        return so_flow::busy_operation ;
    }   

    // disconnect from old
    {
        auto const res = this->disconnect() ;
        if( so_log::log::error(so_flow::no_success( res ),
            "[so_flow::input_slot::connect] : can not disconnect from output slot") )
            return res ;
    }

    // check compatibility
    {
        auto const res = this->is_compatible( os_ptr ) ;
        if( so_log::log::error( so_core::is_not(res),
            "[so_flow::input_slot::connect] : connection failed" ) )
        {
            return so_flow::failed ;
        }
    }

    {
        so_thread::lock_guard_t lk( _mtx ) ;
        _os_ptr = os_ptr ;
    }

    // connect os
    {
        auto const res = _os_ptr->connect( this ) ;
        if( so_flow::no_success(res) )
        {
            so_log::log::error( "[so_flow::input_slot::connect] : connection failed" ) ;
            return res ;
        }
    }

    return so_flow::ok ;
}

//***********************************************************************
so_flow::result input_slot::disconnect( void_t )
{    
    return this->disconnect( _os_ptr ) ;
}

//***********************************************************************
so_flow::result input_slot::disconnect( so_flow::ioutput_slot_ptr_t os_ptr_in ) 
{
    so_flow::ioutput_slot_ptr_t os_ptr = nullptr ;

    {
        so_thread::lock_guard_t lk( _mtx ) ;

        if( _os_ptr != os_ptr_in )
            return so_flow::already_disconnected ;

        if( so_core::is_nullptr( _os_ptr ) )
            return so_flow::already_disconnected ;

        so_move_ptr( os_ptr, _os_ptr ) ;
    }

    {
        auto const res = os_ptr->disconnect( this ) ;
        if( so_log::log::error( so_flow::no_success( res ),
            "[so_flow::input_slot::disconnect] : disconnection failed" ) )
            return res ;
    }

    return so_flow::ok ;
}

//***********************************************************************
void_t input_slot::set_owner( so_flow::inode_ptr_t new_owner ) 
{
    _owner_ptr = new_owner ;
}

//***********************************************************************
so_flow::inode_ptr_t input_slot::get_owner( void_t ) 
{
    return _owner_ptr ;
}


//***********************************************************************
