

#include "sdf_a.h"
#include "sdf_a_plug_factory.h"

#include <snakeoil/memory/global.h>

using namespace sox_presentation ;
using namespace sox_presentation::transitions ;

//*****************************************************
sdf_a::sdf_a( so_gpx::render_system_ptr_t ptr ) noexcept
{
    _gpxr = ptr ;

    _sd_ptr = so_memory::global_t::alloc( sox_presentation::transitions::sdf_a_shared_data_t(),
        "[sdf_a::sdf_a]" ) ;

    _fac_ptr = sox_presentation::transitions::sdf_a_plug_factory_t::create(
        sox_presentation::transitions::sdf_a_plug_factory_t( _sd_ptr ), "[sdf_a::sdf_a]" ) ;

}

//*****************************************************
sdf_a::sdf_a( this_rref_t rhv ) noexcept
{
    so_move_member_ptr( _gpxr, rhv ) ;
    so_move_member_ptr( _sd_ptr, rhv ) ;
    so_move_member_ptr( _fac_ptr, rhv ) ;

    _tid = std::move( rhv._tid ) ;
    rhv._tid = so_gpx::technique_id_t( -1 ) ;
}

//*****************************************************
sdf_a::~sdf_a( void_t ) noexcept
{
    so_memory::global_t::dealloc( _sd_ptr ) ;

    if( _tid != so_gpx::technique_id_t( -1 ) )
    {
        _gpxr->unregister_technique( _tid ) ;
    }
}

//*****************************************************
sdf_a::this_ptr_t sdf_a::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_memory::global_t::alloc( std::move( rhv ), p ) ;
}

//*****************************************************
void_t sdf_a::destroy( this_ptr_t ptr )
{
    so_memory::global_t::dealloc( ptr ) ;
}

//*****************************************************
void_t sdf_a::init( void_t )
{
    if( _tid == so_gpx::technique_id_t( -1 ) )
        _tid = _gpxr->register_technique( _fac_ptr ) ;

    _gpxr->schedule_for_init( _tid ) ;
}

//*****************************************************
void_t sdf_a::set_view_projection( so_math::mat4f_cref_t view, so_math::mat4f_cref_t proj )
{
    _sd_ptr->view = view ;
    _sd_ptr->proj = proj ;
}

//*****************************************************
void_t sdf_a::reload( void_t ) 
{
    _gpxr->schedule_for_reload( _tid, 0 ) ;
}

//*****************************************************
void_t sdf_a::render( sox_presentation::itransition_t::render_type const rt )
{
    so_gpx::schedule_instance_t si ;
    si.render_id = rt == sox_presentation::itransition_t::render_type::mask ? 1 : 0 ;
    _gpxr->schedule( _tid, 0, si ) ;
}

//*****************************************************
void_t sdf_a::release( void_t )
{
    _gpxr->schedule_for_release( _tid ) ;
}

//*****************************************************
so_flow::variable_node_ptr_t sdf_a::get_var_node( void ) 
{
    return &_sd_ptr->vn ;
}

//*****************************************************
void_t sdf_a::update( std::chrono::milliseconds const t ) 
{

}