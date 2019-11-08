//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "predef_framebuffer.h"
#include "predef_framebuffer_plug_factory.h"

#include <snakeoil/flow/variable/variable.hpp>

#include <snakeoil/log/global.h>

using namespace so_gfx ;

//*************************************************************************************
predef_framebuffer::predef_framebuffer( predef_framebuffer_type t, so_gpx::render_system_ptr_t rs )
{
    _type = t ;
    _rs = rs ;  

    {
        _sd = so_gfx::memory::alloc( this_t::shared_data_t(),
            "[predef_framebuffer::predef_framebuffer] : shared_data" ) ;

        _fac_ptr = so_gfx::predef_framebuffer_plug_factory_t::create(
            so_gfx::predef_framebuffer_plug_factory_t( t, _sd ), 
            "[predef_framebuffer::predef_framebuffer] : predef_framebuffer_plug_factory_t" ) ;

        _tid = _rs->register_technique( _fac_ptr ) ;
    }

    // based on the type, create slots
    #if 0
    {
        switch( t )
        {
        case so_gfx::predef_framebuffer_type::toscreen:
            _sd->vs.add( "source_rect", so_flow::variable<so_math::vec4f_t>::create(
                so_math::vec4f_t(), "[predef_framebuffer::predef_framebuffer] : source_rect" ) ) ;
            _sd->vs.add( "dest_rect", so_flow::variable<so_math::vec4f_t>::create(
                so_math::vec4f_t(), "[predef_framebuffer::predef_framebuffer] : dest_rect" ) ) ;
            _sd->vs.add( "source_color_tx", so_flow::variable<so_std::string_t>::create(
                so_std::string_t(), "[predef_framebuffer::predef_framebuffer] : source_color_tx" ) ) ;
            break ;

        case so_gfx::predef_framebuffer_type::vignette:
            _sd->vs.add( "source_rect", so_flow::variable<so_math::vec4f_t>::create(
                so_math::vec4f_t(), "[predef_framebuffer::predef_framebuffer] : source_rect" ) ) ;
            _sd->vs.add( "dest_rect", so_flow::variable<so_math::vec4f_t>::create(
                so_math::vec4f_t(), "[predef_framebuffer::predef_framebuffer] : dest_rect" ) ) ;
            _sd->vs.add( "source_color_tx", so_flow::variable<so_std::string_t>::create(
                so_std::string_t(), "[predef_framebuffer::predef_framebuffer] : source_color_tx" ) ) ;
            _sd->vs.add( "dest_fb", so_flow::variable<so_std::string_t>::create(
                so_std::string_t(), "[predef_framebuffer::predef_framebuffer] : source_color_tx" ) ) ;
            break ;

        default:
            break ;
        }
    }
    #endif
}

//*************************************************************************************
predef_framebuffer::predef_framebuffer( this_rref_t rhv )
{
    so_move_member_ptr( _sd, rhv ) ;
    so_move_member_ptr( _rs, rhv ) ;
    so_move_member_ptr( _fac_ptr, rhv ) ;
    _tid = std::move( rhv._tid ) ;
    rhv._tid = so_gpx::technique_id_t( -1 ) ;
    
    _type = rhv._type ;
}

//*************************************************************************************
predef_framebuffer::~predef_framebuffer( void_t )
{
    if( _tid != so_gpx::technique_id_t( -1 ) )
        _rs->schedule_for_release( _tid ) ;
    
    so_gfx::memory::dealloc( _sd ) ;
}

//*************************************************************************************
predef_framebuffer::this_ref_t predef_framebuffer::operator = ( this_rref_t rhv ) 
{
    so_move_member_ptr( _sd, rhv ) ;
    so_move_member_ptr( _rs, rhv ) ;
    so_move_member_ptr( _fac_ptr, rhv ) ;
    _tid = std::move( rhv._tid ) ;
    rhv._tid = so_gpx::technique_id_t( -1 ) ;

    _type = rhv._type ;

    return *this ;
}

//*************************************************************************************
predef_framebuffer::this_ptr_t predef_framebuffer::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t predef_framebuffer::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t predef_framebuffer::init( so_std::string_cref_t name, size_t const width, 
    size_t const height, size_t const nc )
{
    _sd->fb_name = name ;
    _sd->width = width ;
    _sd->height = height ;
    _sd->num_color = nc ;
    _sd->type = _type ;

    this_t::set_viewport( so_gpu::viewport_2d( 0, 0, width, height ) ) ;
}

//*************************************************************************************
void_t predef_framebuffer::schedule_for_reload( void_t ) 
{
    _rs->schedule_for_reload( _tid, 0 ) ;
}

//*************************************************************************************
void_t predef_framebuffer::set_viewport( so_gpu::viewport_2d_cref_t vp ) 
{
    _sd->vp = vp ;
}

//*************************************************************************************
void_t predef_framebuffer::set_clear_color( so_math::vec4f_cref_t din ) 
{
    _sd->color = din ;
}

//*************************************************************************************
so_flow::variable_set_ptr_t predef_framebuffer::get_varset( void_t ) 
{
    return &(_sd->vs) ;
}

//*************************************************************************************
void_t predef_framebuffer::schedule_for_clear( void_t ) 
{
    so_gpx::schedule_instance_t si ;
    si.render_id = 0 ;
    _rs->schedule( _tid, 0, si ) ;
}

//*************************************************************************************
void_t predef_framebuffer::schedule_for_begin( void_t )
{
    so_gpx::schedule_instance_t si ;
    si.render_id = 1 ;
    _rs->schedule( _tid, 0, si ) ;
}

//*************************************************************************************
void_t predef_framebuffer::schedule_for_end( void_t )
{
    so_gpx::schedule_instance_t si ;
    si.render_id = 2 ;
    _rs->schedule( _tid, 0, si ) ;
}

//*************************************************************************************