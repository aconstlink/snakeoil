//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "node.h"


using namespace so_scene ;
using namespace so_scene::so_node ;

//*******************************************************************
node::node( void_t ) 
{}

//*******************************************************************
node::node( iparent_ptr_t parent_ptr ) : _parent(parent_ptr)
{}

//*******************************************************************
node::node( this_rref_t rhv ) : variable_node_policy_t( std::move(rhv) )
{
    so_move_member_ptr( _parent, rhv ) ;
}

//*******************************************************************
node::this_ref_t node::operator = ( this_rref_t rhv ) 
{
    variable_node_policy_t::operator=( std::move(rhv) ) ;
    so_move_member_ptr( _parent, rhv ) ;
    return *this ;
}

//*******************************************************************
node::~node( void_t ) 
{}

//*******************************************************************
so_scene::result node::apply( so_scene::so_visitor::ivisitor_ptr_t ) 
{
    return so_scene::ok ;
}

//*******************************************************************
iparent_ptr_t node::get_parent( void_t ) 
{
    return _parent ;
}

//*******************************************************************
iparent_ptr_t node::set_parent( iparent_ptr_t parent_ptr ) 
{
    auto old_parent = _parent ;
    _parent = parent_ptr ;
    return old_parent ;
}

//*******************************************************************
so_flow::variable_node_ptr_t node::get_flow_node( void_t )
{
    return variable_node_policy_t::get_flow_node() ;
}

//*******************************************************************
bool_t node::reconnect_flow_node( void_t ) 
{
    return variable_node_policy_t::reconnect_flow_node() ;
}

//*******************************************************************
bool_t node::bind_variable( so_std::string_in_t bp, so_flow::ivariable_ptr_t ptr ) 
{
    return variable_node_policy_t::bind_variable( bp, ptr ) ;
}
