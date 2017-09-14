//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "standard_material.h"

#include <snakeoil/log/global.h>

using namespace so_shade ;
using namespace so_shade::so_material ;

//*************************************************************************************
standard_material::standard_material( void_t ) 
{
}

//*************************************************************************************
standard_material::standard_material( this_rref_t rhv )
{
    _texture_layers = std::move( rhv._texture_layers ) ;
    _color_layers = std::move( rhv._color_layers ) ;
    _normal_layer = rhv._normal_layer ;
}

//*************************************************************************************
standard_material::~standard_material( void_t )
{
}

//*************************************************************************************
standard_material::this_ptr_t standard_material::create( so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
standard_material::this_ptr_t standard_material::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t standard_material::destroy( this_ptr_t ptr )
{
    so_shade::memory::dealloc( ptr ) ;
}

//*************************************************************************************
size_t standard_material::get_num_texture_layers( void_t ) const 
{
    return _texture_layers.size() ;
}

//*************************************************************************************
bool_t standard_material::get_texture_layer_coord_info( size_t layer,
    so_math::vec2f_out_t uv_scale_out, so_math::vec2f_out_t uv_offset_out ) const 
{
    if( _texture_layers.size() <= layer ) return false ;

    auto const & tlayer = _texture_layers[layer] ;

    uv_scale_out = tlayer.uv_scale ;
    uv_offset_out = tlayer.uv_offset ;

    return true ;
}

//*************************************************************************************
size_t standard_material::get_num_lights( void_t ) const 
{
    return _lights ;
}

//*************************************************************************************
bool_t standard_material::has_normal_layer( void_t ) const 
{
    return _normal_layer ;
}

//*************************************************************************************
void_t standard_material::add_texture_layer( void_t ) 
{
    texture_layer_t tl ;
    _texture_layers.push_back( std::move(tl) ) ;
}

//*************************************************************************************
void_t standard_material::add_texture_layer( so_math::vec2f_cref_t uv_scale, 
    so_math::vec2f_cref_t uv_offset ) 
{
    texture_layer_t tl ;
    tl.uv_scale = uv_scale ;
    tl.uv_offset = uv_offset ;
    _texture_layers.push_back( std::move( tl ) ) ;
}

//*************************************************************************************
void_t standard_material::add_normal_layer( void_t ) 
{
    _normal_layer = true ;
}

//*************************************************************************************
void_t standard_material::add_light( void_t ) 
{
    ++_lights ;
}

//*************************************************************************************
so_shade::result standard_material::add_texture( size_t layer, so_shade::texture_variable_in_t var_in ) 
{
    if( _texture_layers.size() <= layer )
    {
        so_log::global::error( "[standard_material::add_texture] : invalid layer index" ) ;
        return so_shade::invalid_argument ;
    }

    texture_info_t ti ;
    ti.tex_var = var_in ;

    _texture_layers[layer].texture_infos.push_back( std::move(ti) ) ;

    return so_shade::ok ;
}

//*************************************************************************************
size_t standard_material::get_num_textures( size_t layer ) const
{
    if( layer >= _texture_layers.size() )
        return 0 ;

    return _texture_layers[layer].texture_infos.size() ;
}

//*************************************************************************************
bool_t standard_material::get_texture( size_t layer, size_t tex, 
    so_shade::texture_variable_out_t var_out )
{
    if( layer >= _texture_layers.size() )
    {
        so_log::global::error( "[standard_material::get_texture] : invalid texture layer index" ) ;
        return false ;
    }

    if( tex >= _texture_layers[layer].texture_infos.size() )
    {
        so_log::global::error( "[standard_material::get_texture] : invalid texture index" ) ;
        return false ;
    }

    var_out = _texture_layers[layer].texture_infos[tex].tex_var ;
    return true ;
}

//*************************************************************************************
void_t standard_material::add_color_layer( so_math::vec3f_cref_t c ) 
{
    this_t::color_layer_t cl ;
    cl.color = c ;
    _color_layers.push_back( cl ) ;
}

//*************************************************************************************
size_t standard_material::get_num_color_layers( void_t ) const 
{
    return _color_layers.size() ;
}

//*************************************************************************************
bool_t standard_material::get_color_layer( size_t i, so_math::vec3f_out_t vout ) const 
{
    if( i >= _color_layers.size() ) 
    {
        so_log::global::warning( 
            "[so_shade::standard_material::get_color_layer] : incorrect index" ) ;
        return false ;
    }

    vout = _color_layers[i].color ;

    return true ;
}

//*************************************************************************************
void_t standard_material::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
