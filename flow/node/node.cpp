//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "node.h"
#include "../variable/ivariable.h"
#include "../slot/iinput_slot.h"
#include "../slot/ioutput_slot.h"

#include <snakeoil/log/log.h>

#include <algorithm>

using namespace so_flow ;

//***********************************************************************
node::node( void_t ) 
{}

//***********************************************************************
node::node( this_rref_t rhv ) : node_input_policy(std::move(rhv)), node_output_policy(std::move(rhv))
{
    inputs_t::for_each( [&]( so_flow::iinput_slot_ptr_t sptr )
    {
        sptr->set_owner(this) ;
    } ) ;

    outputs_t::for_each( [&]( so_flow::key_cref_t, outputs_t::store_data_ref_t item )
    {
        item.sptr->set_owner( this ) ;
    } ) ;
}

//***********************************************************************
node::this_ref_t node::operator = ( this_rref_t rhv )
{
    node_input_policy::operator = ( std::move( rhv ) ) ;
    node_output_policy::operator = ( std::move( rhv ) ) ;

    inputs_t::for_each( [&]( so_flow::iinput_slot_ptr_t sptr )
    {
        sptr->set_owner( this ) ;
    } ) ;

    outputs_t::for_each( [&]( so_flow::key_cref_t, outputs_t::store_data_ref_t item )
    {
        item.sptr->set_owner( this ) ;
    } ) ;

    return *this ;
}

//***********************************************************************
node::~node( void_t )
{}

//***********************************************************************
so_flow::result node::create_input_slot( so_std::string_cref_t name, so_flow::ivariable_ptr_t var_ptr )
{
    if( so_core::is_nullptr( var_ptr ) )
        return so_flow::invalid_pointer ;

    auto * is_ptr = var_ptr->create_input_slot( "[so_flow::node::create_input_slot]" ) ; 
    
    auto const res = inputs_t::add_slot( name, is_ptr ) ;
    if( so_flow::no_success( res ) )
    {
        so_log::log::error( "[so_flow::node::create_input_slot] : can not create slot." ) ;
        is_ptr->destroy() ;
        return res ;
    }

    is_ptr->set_owner( this ) ;

    return so_flow::ok ;
}

//***********************************************************************
so_flow::result node::create_output_slot( so_std::string_cref_t name, so_flow::ivariable_ptr_t var_ptr )
{
    if( so_core::is_nullptr( var_ptr ) )
        return so_flow::invalid_pointer ;

    auto * s_ptr = var_ptr->create_output_slot( "[so_flow::node::create_output_slot]" ) ;

    auto const res = outputs_t::add_slot( name, s_ptr ) ;
    if( so_flow::no_success( res ) )
    {
        so_log::log::error( "[so_flow::node::create_output_slot] : can not create slot." ) ;
        s_ptr->destroy() ;
        return res ;
    }

    s_ptr->set_owner( this ) ;

    return so_flow::ok ;
}

//***********************************************************************
so_flow::result node::connect( so_flow::key_cref_t key, so_flow::ioutput_slot_ptr_t os_ptr )
{
    inputs_t::handle_t is_hnd ;
    {        
        auto res = inputs_t::find_and_touch( key, is_hnd ) ;
        if( so_log::log::error( so_core::is_not( res ),
            "[so_flow::node::connect] : can not find input slot : " + key ) )
            return so_flow::invalid_argument ;
    }
    
    auto const res = is_hnd->connect( os_ptr ) ;
    if( so_log::log::error( so_flow::no_success( res ),
        "[so_flow::node::connect] : slot connect" ) )
        return res ;
    
    return so_flow::ok ;
}

//***********************************************************************
so_flow::result node::connect( so_flow::key_cref_t key, so_flow::iinput_slot_ptr_t is_ptr )
{
    outputs_t::handle_t shnd ;
    {
        auto const res = outputs_t::find_and_touch( key, shnd ) ;
        if( so_core::is_not(res) )
        {
            so_log::log::error( 
                "[so_flow::node::connect] : can not find output slot : " + key ) ;

            return so_flow::invalid_argument ;
        }
    }

    auto const res = shnd->connect( is_ptr ) ;
    if( so_log::log::error( so_flow::no_success( res ),
        "[so_flow::node::connect] : slot connect" ) )
        return res ;

    return so_flow::ok ;
}

//***********************************************************************
so_flow::result node::connect_this_output_to_that_input( so_flow::key_cref_t on_os, 
    so_flow::key_cref_t on_is, so_flow::inode_ptr_t other_node_ptr ) 
{
    if( so_core::is_nullptr(other_node_ptr) )
        return so_flow::invalid_argument ; 
    
    outputs_t::handle_t shnd ;
    {
        auto const res = outputs_t::find_and_touch( on_os, shnd ) ;
        if( so_core::is_not( res ) )
        {
            so_log::log::error(
                "[so_flow::node::connect] : can not find output slot : " + on_os ) ;

            return so_flow::invalid_argument ;
        }
    }

    return other_node_ptr->connect( on_is, shnd.get_ptr() ) ;
}

//***********************************************************************
so_flow::result node::on_trigger( so_flow::inode::nodes_ref_t nodes_out )
{
    if( so_core::is_not( this->on_trigger() ) )
    {
        return so_flow::ok ;
    }

    // for a pure output slot node, the tigger need to be at least 1
    size_t const max_sem = std::max( size_t(1), inputs_t::get_num_slots() ) ;

    _triggered.increment( max_sem ) ;
    
    // if this node has been triggered already, there
    // is no need to trigger all outputs again.
    if( _triggered > 1 )
        return so_flow::ok ;

    outputs_t::for_each( [&]( so_flow::key_cref_t, outputs_t::store_data_ref_t item )
    {
        // if an output is already signaled via another 
        // path, it is not required to do it again.
        // @note this might be obsolete due to _triggered in node on_trigger
        if( so_core::is_not(item.signaled) )
        {
            item.signaled = true ;
            item.sptr->get_all_connected_owner( nodes_out ) ;
        }
    }) ;
    return so_flow::ok ;
}

//***********************************************************************
so_flow::result node::on_update( so_flow::inode::nodes_ref_t nodes_out ) 
{
    auto const res = _triggered.decrement([=]( size_t sem ){ return sem == 0 ; }) ;

    // we only call update and collect all connected nodes when
    // the last on_update call is made of all on_triggers
    if( so_core::is_not(res) ) return so_flow::ok ;
    
    this->on_update() ;
        
    outputs_t::for_each( [&]( so_flow::key_cref_t, outputs_t::store_data_ref_t item )
    {
        if( item.signaled )
        {
            // 2. update output slots
            item.signaled = false ;
            item.sptr->update() ;

            // 3. return all next output slots
            item.sptr->get_all_connected_owner( nodes_out ) ;
        }
            
    } ) ;

    return so_flow::ok ;
}

//***********************************************************************
bool_t node::on_trigger( void_t ) 
{ 
    return true ; 
}
