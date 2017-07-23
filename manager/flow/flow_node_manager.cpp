//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "flow_node_manager.h"

using namespace so_manager ;

//**********************************************************************************************
flow_node_manager::flow_node_manager( void_t ) 
{
}

//**********************************************************************************************
flow_node_manager::flow_node_manager( this_rref_t rhv )
{
    _flow_node_mgr = std::move( rhv._flow_node_mgr ) ;
    _counter = rhv._counter ;
}

//**********************************************************************************************
flow_node_manager::~flow_node_manager( void_t )
{
}

//**********************************************************************************************
flow_node_manager::this_ptr_t flow_node_manager::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_manager::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************************
void_t flow_node_manager::destroy( this_ptr_t ptr )
{
    so_manager::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
bool_t flow_node_manager::acquire( so_manager::key_cref_t key_in,
    so_resource::purpose_cref_t p, handle_out_t hnd_out )
{
    return _flow_node_mgr.acquire( key_in, p, hnd_out ) ;
}

//**********************************************************************************************
bool_t flow_node_manager::release( handle_rref_t hnd )
{
    return _flow_node_mgr.release( std::move(hnd) ) ;
}

//**********************************************************************************************
so_manager::result flow_node_manager::insert( so_manager::key_cref_t key_in, 
    manage_params_cref_t mp )
{
    store_item_t si ;
    // fill si

    auto si_ptr = so_manager::memory::alloc( std::move(si), 
        "[so_manager::flow_node_manager::insert] : linear spline store item for " + key_in ) ;

    auto const res = _flow_node_mgr.insert( key_in, si_ptr ) ;
    if( so_log::log::error( so_resource::no_success( res ),
        "[so_manager::flow_node_manager::insert] : insert" ) )
    {
        so_manager::memory::dealloc( si_ptr ) ;
        return so_manager::key_already_in_use;
    }

    return so_manager::ok ;
}

//**********************************************************************************************