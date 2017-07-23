//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "node.h"

using namespace so_imex ;
using namespace so_imex::so_node ;

//*************************************************************************************
node::node( void_t ) 
{

}

//*************************************************************************************
node::node( this_rref_t rhv ) 
{
    so_move_member_ptr( _parent_ptr, rhv ) ;
}

//*************************************************************************************
node::~node( void_t ) 
{}

//*************************************************************************************
void_t node::set_parent( iparent_ptr_t pptr ) 
{
    _parent_ptr = pptr ;
}
