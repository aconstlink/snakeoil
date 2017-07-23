//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "variable_node_policy.h"

#include <snakeoil/flow/node/variable/variable_node.h>
#include <snakeoil/variable/variable_set/variable_set.h>

#include <snakeoil/log/log.h>

using namespace so_scene ;
using namespace so_scene::so_node ;

//*************************************************************************************
variable_node_policy::variable_node_policy( void_t )
{
    _var_node_ptr = so_flow::variable_node_t::create( 
        "[variable_node_policy::variable_node_policy] : variable_node" ) ;

    _var_set_ptr = so_var::variable_set_t::create(
        "[variable_node_policy::variable_node_policy] : variable_set" ) ;
}

//*************************************************************************************
variable_node_policy::variable_node_policy( this_rref_t rhv )
{
    so_move_member_ptr( _var_node_ptr, rhv ) ;
    so_move_member_ptr( _var_set_ptr, rhv ) ;
}

//*************************************************************************************
variable_node_policy::this_ref_t variable_node_policy::operator = ( this_rref_t rhv ) 
{
    so_move_member_ptr( _var_node_ptr, rhv ) ;
    so_move_member_ptr( _var_set_ptr, rhv ) ;

    return *this ;
}

//*************************************************************************************
variable_node_policy::~variable_node_policy( void_t )
{
    so_flow::variable_node_t::destroy( _var_node_ptr ) ;
    so_var::variable_set_t::destroy( _var_set_ptr ) ;
}

//*************************************************************************************
bool_t variable_node_policy::reconnect_flow_node( void_t )
{
    auto const res = _var_node_ptr->create_input_connections( *_var_set_ptr ) ;
    if( so_flow::no_success( res ) )
    {
        return false ;
    }

    return true ;
}

//*************************************************************************************
so_flow::variable_node_ptr_t variable_node_policy::get_flow_node( void_t )
{
    return _var_node_ptr ;
}

//*************************************************************************************
bool_t variable_node_policy::bind_variable( so_std::string_in_t bp, so_var::ivariable_ptr_t ptr ) 
{
    if( so_core::is_nullptr( _var_set_ptr ) )
        return false ;

    auto const res = _var_set_ptr->add( bp, ptr ) ;
    so_log::log::error_and_exit( so_var::no_success( res ),
        "[renderable::bind_object_matrix] : rebind not handled" ) ;

    return true ;
}
