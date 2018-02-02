//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "node.h"

#include "../structs/inode_listener.h"

using namespace so_ui ;
using namespace so_ui::so_node ;

//*******************************************************************
node::node( void_t ) 
{}

//*******************************************************************
node::node( iparent_ptr_t parent_ptr ) : _parent(parent_ptr)
{}

//*******************************************************************
node::node( this_rref_t rhv ) : variable_node_component_t( std::move(rhv) ), components_component( std::move(rhv) )
{
    so_move_member_ptr( _parent, rhv ) ;

    _listeners = std::move( rhv._listeners ) ;
    for( auto * l : _listeners )
    {
        l->on_move( this ) ;
    }
}

//*******************************************************************
node::this_ref_t node::operator = ( this_rref_t rhv ) 
{
    variable_node_component_t::operator=( std::move( rhv ) ) ;
    components_component_t::operator=( std::move( rhv ) ) ;

    so_move_member_ptr( _parent, rhv ) ;

    _listeners = std::move( rhv._listeners ) ;
    for( auto * l : _listeners )
    {
        l->on_move( this ) ;
    }

    return *this ;
}

//*******************************************************************
node::~node( void_t ) 
{
    for( auto * l : _listeners )
    {
        l->on_move( nullptr ) ;
    }
}

//*******************************************************************
so_ui::result node::apply( so_ui::so_visitor::ivisitor_ptr_t ) 
{
    return so_ui::ok ;
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
void_t node::register_listener( so_ui::inode_listener_ptr_t l )
{
    auto const iter = std::find( _listeners.begin(), _listeners.end(), l ) ;
    if( iter != _listeners.end() )
        return ;

    _listeners.push_back( l ) ;

    l->on_move( this ) ;
}

//*******************************************************************
void_t node::remove_listener( so_ui::inode_listener_ptr_t l )
{
    auto const iter = std::find( _listeners.begin(), _listeners.end(), l ) ;
    if( iter == _listeners.end() )
        return ;

    _listeners.erase( iter ) ;
}

//*******************************************************************
so_flow::variable_node_ptr_t node::get_flow_node( void_t )
{
    return variable_node_component_t::get_flow_node() ;
}

//*******************************************************************
bool_t node::reconnect_flow_node( void_t ) 
{
    return variable_node_component_t::reconnect_flow_node() ;
}

//*******************************************************************
bool_t node::bind_variable( so_std::string_in_t bp, so_flow::ivariable_ptr_t ptr ) 
{
    return variable_node_component_t::bind_variable( bp, ptr ) ;
}

//*******************************************************************
bool_t node::add_component( so_ui::icomponent_ptr_t cptr )
{
    return components_component::add_component( cptr ) ;
}