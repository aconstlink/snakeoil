//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "spline_manager.h"

using namespace so_manager ;

//**********************************************************************************************
spline_manager::spline_manager( void_t ) 
{
}

//**********************************************************************************************
spline_manager::spline_manager( this_rref_t rhv )
{
    _linears_res_mgr = std::move( rhv._linears_res_mgr ) ;

    _counter = rhv._counter ;
}

//**********************************************************************************************
spline_manager::~spline_manager( void_t )
{
}

//**********************************************************************************************
spline_manager::this_ptr_t spline_manager::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_manager::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************************
void_t spline_manager::destroy( this_ptr_t ptr )
{
    so_manager::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
bool_t spline_manager::acquire( so_manager::key_cref_t key_in,
    so_resource::purpose_cref_t p, linears_handle_out_t hnd_out )
{
    return _linears_res_mgr.acquire( key_in, p, hnd_out ) ;
}

//**********************************************************************************************
bool_t spline_manager::release( linears_handle_rref_t hnd )
{
    return _linears_res_mgr.release( std::move(hnd) ) ;
}

//**********************************************************************************************
so_manager::result spline_manager::insert( so_manager::key_cref_t key_in, 
    linears_manage_params_cref_t mp )
{
    linear_spline_store_item si ;
    // fill si

    auto si_ptr = so_manager::memory::alloc( std::move(si), 
        "[so_manager::spline_manager::insert] : linear spline store item for " + key_in ) ;

    auto const res = _linears_res_mgr.insert( key_in, si_ptr ) ;
    if( so_log::log::error( so_resource::no_success( res ),
        "[so_manager::spline_manager::insert] : insert" ) )
    {
        so_manager::memory::dealloc( si_ptr ) ;
        return so_manager::key_already_in_use;
    }

    return so_manager::ok ;
}

//**********************************************************************************************