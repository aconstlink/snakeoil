

#include "post.h"
#include "post_plug_factory.h"

#include <snakeoil/memory/global.h>

using namespace sox_presentation ;

//*****************************************************
post::post( so_gpx::render_system_ptr_t ptr ) 
{
    _gpxr = ptr ;

    _sd_ptr = so_memory::global_t::alloc( sox_presentation::post_shared_data_t(),
        "[post::post]" ) ;

    _fac_ptr = sox_presentation::post_plug_factory_t::create(
        sox_presentation::post_plug_factory_t( _sd_ptr ), "[post::post]" ) ;

}

//*****************************************************
post::post( this_rref_t rhv )
{
    so_move_member_ptr( _gpxr, rhv ) ;
    so_move_member_ptr( _sd_ptr, rhv ) ;
    so_move_member_ptr( _fac_ptr, rhv ) ;

    _tid = std::move( rhv._tid ) ;
    rhv._tid = so_gpx::technique_id_t( -1 ) ;
}

//*****************************************************
post::~post( void_t )
{
    so_memory::global_t::dealloc( _sd_ptr ) ;

    if( _tid != so_gpx::technique_id_t( -1 ) )
    {
        _gpxr->unregister_technique( _tid ) ;
    }
}

//*****************************************************
post::this_ptr_t post::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_memory::global_t::alloc( std::move( rhv ), p ) ;
}

//*****************************************************
void_t post::destroy( this_ptr_t ptr )
{
    so_memory::global_t::dealloc( ptr ) ;
}

//*****************************************************
void_t post::init( so_std::string_cref_t fb0, so_std::string_cref_t fb1,
    so_std::string_cref_t fbx, so_std::string_cref_t fbm,
    so_std::string_cref_t target )
{
    if( _tid == so_gpx::technique_id_t( -1 ) )
        _tid = _gpxr->register_technique( _fac_ptr ) ;

    _sd_ptr->fb0_name = fb0 ;
    _sd_ptr->fb1_name = fb1 ;
    _sd_ptr->fbx_name = fbx ;
    _sd_ptr->fbm_name = fbm ;
    _sd_ptr->trg_name = target ;
}

//*****************************************************
void_t post::set_view_projection( so_math::mat4f_cref_t view, so_math::mat4f_cref_t proj )
{
    _sd_ptr->view = view ;
    _sd_ptr->proj = proj ;
}

//*****************************************************
void_t post::reload( void_t ) 
{
    _gpxr->schedule_for_reload( _tid, 0 ) ;
}

//*****************************************************
void_t post::render( bool_t const transition )
{
    so_gpx::schedule_instance_t si ;
    si.render_id = transition ? 1 : 0 ;
    _gpxr->schedule( _tid, 0, si ) ;
}

//*****************************************************
void_t post::release( void_t )
{
    _gpxr->schedule_for_release( _tid ) ;
}

//*****************************************************
so_flow::variable_node_ptr_t post::get_var_node( void ) 
{
    return &_sd_ptr->vn ;
}

