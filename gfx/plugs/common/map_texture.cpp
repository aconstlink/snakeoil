//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "map_texture.h"
#include "map_texture_plug_factory.h"

using namespace so_gfx ;

//*************************************************************************************
map_texture::map_texture( so_std::string_cref_t name, so_gpx::render_system_ptr_t rs )
{
    _sd = so_gfx::memory::alloc( this_t::shared_data_t(),
        "[map_texture::map_texture] : shared_data" ) ;
    _sd->name = name ;
    _rs = rs ;

    _tid = _rs->register_technique( map_texture_plug_factory_t::create(
        map_texture_plug_factory_t( _sd ),
        "[map_texture::map_texture] : map_texture_plug_factory_t" ) ) ;
}

//*************************************************************************************
map_texture::map_texture( this_rref_t rhv )
{
    so_move_member_ptr( _sd, rhv ) ;
    so_move_member_ptr( _rs, rhv ) ;
    _tid = std::move( rhv._tid ) ;
}

//*************************************************************************************
map_texture::~map_texture( void_t )
{

}

//*************************************************************************************
map_texture::this_ptr_t map_texture::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t map_texture::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t map_texture::schedule( void_t )
{

}

//*************************************************************************************