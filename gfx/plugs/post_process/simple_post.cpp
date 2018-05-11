//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "simple_post.h"
#include "simple_post_plug_factory.h"

using namespace so_gfx ;

//*************************************************************************************
simple_post::simple_post( so_gpx::render_system_ptr_t rs )
{
    {
        _sd = so_gfx::memory::alloc( this_t::shared_data_t(),
            "[map_texture::map_texture] : shared_data" ) ;

        //_sd->vs.add
    }

    _rs = rs ;

    _tid = _rs->register_technique( simple_post_plug_factory_t::create(
        simple_post_plug_factory_t( _sd ),
        "[map_texture::map_texture] : map_texture_plug_factory_t" ) ) ;
}

//*************************************************************************************
simple_post::simple_post( this_rref_t rhv )
{
    so_move_member_ptr( _sd, rhv ) ;
    so_move_member_ptr( _rs, rhv ) ;
    _tid = std::move( rhv._tid ) ;
}

//*************************************************************************************
simple_post::~simple_post( void_t )
{

}

//*************************************************************************************
simple_post::this_ptr_t simple_post::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t simple_post::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t simple_post::schedule( void_t )
{

}

//*************************************************************************************
void_t simple_post::set_source_color_name( so_std::string_cref_t name ) 
{
    _sd->name = name ;
}

//*************************************************************************************
so_flow::variable_set_ptr_t simple_post::get_varset( void_t ) 
{
    return &(_sd->vs) ;
}