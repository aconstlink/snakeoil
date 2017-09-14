//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "ilogic.h"
#include "logic_node.h"

#include <snakeoil/log/global.h>

using namespace so_flow ;

//***********************************************************************
logic_node::logic_node( so_flow::ilogic_ptr_t lptr ) 
{
    _logic_ptr = lptr ;
}

//***********************************************************************
logic_node::logic_node( this_rref_t rhv )
{
    so_move_member_ptr( _logic_ptr, rhv ) ;
}

//***********************************************************************
logic_node::~logic_node( void_t )
{
    if( so_core::is_not_nullptr(_logic_ptr) )
        _logic_ptr->destroy() ;
}

//***********************************************************************
logic_node::this_ptr_t logic_node::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_flow::memory::alloc( std::move( rhv), p ) ; 
}

//***********************************************************************
void_t logic_node::destroy( this_ptr_t ptr )
{
    so_flow::memory::dealloc( ptr ) ;
}

//***********************************************************************
bool_t logic_node::on_trigger( void_t )
{
    if( so_core::is_nullptr(_logic_ptr) )
        return false ;

    return _logic_ptr->on_trigger( this ) ;
}

//***********************************************************************
bool_t logic_node::on_update( void_t )
{
    if( so_core::is_nullptr(_logic_ptr) )
        return false ;

    return _logic_ptr->on_update( this ) ;
}

//***********************************************************************
void_t logic_node::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
