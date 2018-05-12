//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "predef_post.h"
#include "predef_post_plug_factory.h"

#include <snakeoil/flow/variable/variable.hpp>

#include <snakeoil/log/global.h>

using namespace so_gfx ;

//*************************************************************************************
predef_post::predef_post( predef_post_type t, so_gpx::render_system_ptr_t rs )
{
    _type = t ;
    _rs = rs ;  

    {
        _sd = so_gfx::memory::alloc( this_t::shared_data_t(),
            "[predef_post::predef_post] : shared_data" ) ;

        _fac_ptr = so_gfx::predef_post_plug_factory_t::create(
            so_gfx::predef_post_plug_factory_t( t, _sd ), 
            "[predef_post::predef_post] : predef_post_plug_factory_t" ) ;

        _tid = _rs->register_technique( _fac_ptr ) ;
    }

    // based on the type, create slots
    {
        switch( t )
        {
        case so_gfx::predef_post_type::toscreen:
            _sd->vs.add( "source_rect", so_flow::variable<so_math::vec4f_t>::create(
                so_math::vec4f_t(), "[predef_post::predef_post] : source_rect" ) ) ;
            _sd->vs.add( "dest_rect", so_flow::variable<so_math::vec4f_t>::create(
                so_math::vec4f_t(), "[predef_post::predef_post] : dest_rect" ) ) ;
            _sd->vs.add( "source_color_tx", so_flow::variable<so_std::string_t>::create(
                so_std::string_t(), "[predef_post::predef_post] : source_color_tx" ) ) ;
            break ;

        case so_gfx::predef_post_type::vignette:
            _sd->vs.add( "source_rect", so_flow::variable<so_math::vec4f_t>::create(
                so_math::vec4f_t(), "[predef_post::predef_post] : source_rect" ) ) ;
            _sd->vs.add( "dest_rect", so_flow::variable<so_math::vec4f_t>::create(
                so_math::vec4f_t(), "[predef_post::predef_post] : dest_rect" ) ) ;
            _sd->vs.add( "source_color_tx", so_flow::variable<so_std::string_t>::create(
                so_std::string_t(), "[predef_post::predef_post] : source_color_tx" ) ) ;
            _sd->vs.add( "dest_fb", so_flow::variable<so_std::string_t>::create(
                so_std::string_t(), "[predef_post::predef_post] : source_color_tx" ) ) ;
            break ;

        default:
            break ;
        }
    }
}

//*************************************************************************************
predef_post::predef_post( this_rref_t rhv )
{
    so_move_member_ptr( _sd, rhv ) ;
    so_move_member_ptr( _rs, rhv ) ;
    so_move_member_ptr( _fac_ptr, rhv ) ;
    _tid = std::move( rhv._tid ) ;
    rhv._tid = so_gpx::technique_id_t( -1 ) ;
    
    _type = rhv._type ;
}

//*************************************************************************************
predef_post::~predef_post( void_t )
{
    if( _tid != so_gpx::technique_id_t( -1 ) )
        _rs->schedule_for_release( _tid ) ;
    
    so_gfx::memory::dealloc( _sd ) ;
}

//*************************************************************************************
predef_post::this_ptr_t predef_post::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t predef_post::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t predef_post::schedule( void_t )
{
    so_gpx::schedule_instance_t si ;
    si.render_id = 0 ;
    _rs->schedule( _tid, 0, si ) ;
}

//*************************************************************************************
void_t predef_post::schedule_for_reload( void_t ) 
{
    _rs->schedule_for_reload( _tid, 0 ) ;
}

//*************************************************************************************
void_t predef_post::set_source_color_name( so_std::string_cref_t name ) 
{
    so_flow::variable<so_std::string_t> * v ;
    if( so_core::is_not( _sd->vs.get_variable<so_std::string_t>( "source_color_tx", v ) ) )
    {
        so_log::global_t::error( "[predef_post::set_source_color_name] : no variable" ) ;
        return ;
    }

    v->set_data( name ) ;
}

//*************************************************************************************
void_t predef_post::set_source_rect( so_math::vec4f_cref_t din ) 
{
    so_flow::variable<so_math::vec4f_t> * v ;
    if( so_core::is_not( _sd->vs.get_variable<so_math::vec4f_t>( "source_rect", v ) ) )
    {
        so_log::global_t::error( "[predef_post::set_source_rect] : no variable" ) ;
        return ;
    }

    v->set_data( din ) ;
}

//*************************************************************************************
void_t predef_post::set_dest_rect( so_math::vec4f_cref_t din ) 
{
    so_flow::variable<so_math::vec4f_t> * v ;
    if( so_core::is_not( _sd->vs.get_variable<so_math::vec4f_t>( "dest_rect", v ) ) )
    {
        so_log::global_t::error( "[predef_post::set_dest_rect] : no variable" ) ;
        return ;
    }

    v->set_data( din ) ;
}

//*************************************************************************************
void_t predef_post::set_dest_rect( so_gpu::viewport_2d_cref_t vp ) 
{
    this_t::set_dest_rect( vp.get_as_vec4<float_t>() ) ;
}

//*************************************************************************************
so_flow::variable_set_ptr_t predef_post::get_varset( void_t ) 
{
    return &(_sd->vs) ;
}