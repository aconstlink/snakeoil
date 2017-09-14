//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "slot_group.h"

#include <snakeoil/core/macros/move.h>

#include <snakeoil/log/global.h>

using namespace so_shade ;

//**********************************************************************************************
slot_group::slot_group( void_t ) 
{}

//**********************************************************************************************
slot_group::slot_group( this_rref_t rhv )
{
    _slots = std::move(rhv._slots) ;
}

//**********************************************************************************************
slot_group::~slot_group( void_t )
{
    so_log::global::warning( _slots.size() > 0, 
        "[slot_group::~slot_group] : not all slots disconnected." ) ; 
}

//**********************************************************************************************
bool_t slot_group::has_any_connection( void_t ) const 
{
    return _slots.size() > 0 ;
}

//**********************************************************************************************
bool_t slot_group::exchange( this_ptr_t which_ptr, this_ptr_t with_ptr ) 
{
    auto found = std::find( _slots.begin(), _slots.end(), which_ptr ) ;
    if( found == _slots.end() ) 
        return false ;

    *found = with_ptr ;

    return true ;
}

//**********************************************************************************************
so_shade::result slot_group::add_slot( so_shade::islot_ptr_t sptr )
{
    if( so_core::is_nullptr(sptr) )
        return so_shade::invalid_argument ;

    auto found = std::find( _slots.begin(), _slots.end(), sptr ) ;
    if( found != _slots.end() )
        return so_shade::ok ;

    _slots.push_back( sptr ) ;

    return so_shade::ok ;
}

//**********************************************************************************************
so_shade::result slot_group::remove_slot( islot_ptr_t sptr ) 
{
    if( so_core::is_nullptr(sptr) )
        return so_shade::ok ;

    auto iter = std::find( _slots.begin(), _slots.end(), sptr ) ;
    if( iter == _slots.end() ) 
        return so_shade::invalid_argument ;

    _slots.erase( iter ) ;

    return so_shade::ok ;
}

//**********************************************************************************************
so_shade::result slot_group::remove_all_slots( void_t ) 
{
    _slots.clear() ;
    return so_shade::ok ;
}
