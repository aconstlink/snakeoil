#include "resource_bridge.h"

#include "resource_bridge_plug_factory.h"

#include <snakeoil/gpx/system/render_system.h>

using namespace so_gfx ;

//*************************************************************************************
so_gfx::resource_bridge::resource_bridge( so_gpx::render_system_ptr_t rs )
{
    _rs = rs ;

    _sd = so_memory::global_t::alloc( shared_data_t(), 
        "[resource_bridge::resource_bridge] : shared_data"  ) ;

    _fac_ptr = so_gfx::resource_bridge_plug_factory_t::create(
        so_gfx::resource_bridge_plug_factory_t( _sd ),
        "[resource_bridge::resource_bridge] : resource_bridge_plug_factory_t" ) ;

    _tid = _rs->register_technique( _fac_ptr ) ;
}

//*************************************************************************************
so_gfx::resource_bridge::resource_bridge( this_rref_t rhv )
{
    so_move_member_ptr( _sd, rhv ) ;
    so_move_member_ptr( _fac_ptr, rhv ) ;

    _tid = std::move( rhv._tid ) ;
    so_move_member_ptr( _rs, rhv ) ;
}

//*************************************************************************************
so_gfx::resource_bridge::~resource_bridge( void_t )
{
    if( so_core::is_not_nullptr( _sd ) )
    {
        so_memory::global_t::dealloc( _sd ) ;
    }

    // factory will be destroyed automatically
}

//*************************************************************************************
resource_bridge::this_ptr_t so_gfx::resource_bridge::create( 
    this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_memory::global_t::alloc( this_t( std::move( rhv ) ), p ) ;
}

//*************************************************************************************
void_t so_gfx::resource_bridge::destroy( this_ptr_t ptr )
{
    return so_memory::global_t::dealloc( ptr ) ;
}

//*************************************************************************************
void_t so_gfx::resource_bridge::upload_image( so_std::string_cref_t imex_name, so_std::string_cref_t gpu_name )
{
    shared_data_t::data_item_t di ;
    di.m = shared_data_t::mode::com_upload_image ;
    di.name_a = imex_name ;
    di.name_b = gpu_name ;

    {
        so_thread::lock_guard_t lk( _sd->mtx ) ;
        _sd->dis.push_back( di ) ;
    }

    _rs->schedule( _tid ) ;

    return void_t();
}

//*************************************************************************************
