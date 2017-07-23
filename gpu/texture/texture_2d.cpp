//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "texture_2d.h"

using namespace so_gpu ;

//*****************************************************************************************************
texture_2d::this_ptr_t texture_2d::create( this_rref_t rhv ) 
{
    return this_t::create( std::move(rhv), "[so_gpu::texture_2d::create]" ) ;
}

//*****************************************************************************************************
texture_2d::this_ptr_t texture_2d::create( this_rref_t rhv, std::string const & purpose ) 
{
    return so_gpu::memory::alloc( std::move(rhv), purpose ) ;
}

//*****************************************************************************************************
void_t texture_2d::destroy( this_ptr_t ptr )
{
    so_gpu::memory::dealloc( ptr ) ;
}

//*****************************************************************************************************
texture_2d::texture_2d( iimage_2d_ptr_t ptr ) 
{
    so_assert( ptr != nullptr ) ;
    _image_ptr = ptr ;

    _region = so_gpu::viewport_2d( 0, 0, _image_ptr->get_width(), _image_ptr->get_height() ) ;
}

//*****************************************************************************************************
texture_2d::texture_2d( this_rref_t rhv ) : base_t( std::move( rhv ) )
{
    _image_ptr = rhv._image_ptr ;
    rhv._image_ptr = nullptr ;

    _region = rhv._region ;
    _tex_coord_info = rhv._tex_coord_info ;

    _wrap_s = rhv._wrap_s ;
    _wrap_t = rhv._wrap_t ;

    _filter_s = rhv._filter_s ;
    _filter_t = rhv._filter_t ;
}

//*****************************************************************************************************
texture_2d::~texture_2d( void_t ) 
{

}

//*****************************************************************************************************
so_math::vec2f_t texture_2d::get_tex_dim( void_t ) const
{
    return _tex_coord_info.zw() ;
}

//*****************************************************************************************************
so_math::vec2f_t texture_2d::get_tex_offset( void_t ) const
{
    return _tex_coord_info.xy() ;
}

//*****************************************************************************************************
so_math::vec4f_t texture_2d::get_tex_coord_info( void_t ) const
{
    return _tex_coord_info ;
}

//*****************************************************************************************************
so_gpu::viewport_2d_cref_t texture_2d::get_region( void_t ) const
{
    return _region ;
}

//*****************************************************************************************************
void_t texture_2d::set_region( so_gpu::viewport_2d_cref_t region )
{
    _region = region ;

    if( _image_ptr == nullptr ) return ;
    
    so_math::vec2f_t const off = so_math::vec2f_t(
        float_t(_region.get_x()) / float_t(_image_ptr->get_width()), 
        float_t(_region.get_y()) / float_t(_image_ptr->get_height())
    ) ;

    so_math::vec2f_t const dim = so_math::vec2f_t(
        float_t(_region.get_width()) / float_t(_image_ptr->get_width()), 
        float_t(_region.get_height()) / float_t(_image_ptr->get_height())
    ) ;
    
    _tex_coord_info = so_math::vec4f_t( off, dim ) ;
}

//*****************************************************************************************************
void_t texture_2d::reset_region( void_t ) 
{
    _region = so_gpu::viewport_2d( 0, 0, _image_ptr->get_width(), _image_ptr->get_height() ) ;
    _tex_coord_info = so_math::vec4f_t(float_t(0), float_t(0), float_t(1), float_t(1) ) ;
}

//*****************************************************************************************************
void_t texture_2d::set_texture_wrap( texture_wrap_type s, texture_wrap_type t ) 
{
    _wrap_s = s ;
    _wrap_t = t ;
    base_t::set_state_changed( true ) ;
}

//*****************************************************************************************************
void_t texture_2d::set_texture_filter( texture_filter_type s, texture_filter_type t ) 
{
    _filter_s = s ;
    _filter_t = t ;
    base_t::set_state_changed( true ) ;
}

//*****************************************************************************************************
texture_2d::wrap_st_t texture_2d::get_texture_wrap( void_t ) const 
{
    return wrap_st_t( _wrap_s, _wrap_t ) ;
}

//*****************************************************************************************************
texture_2d::filter_st_t texture_2d::get_texture_filter( void_t ) const 
{
    return filter_st_t( _filter_s, _filter_t ) ;
}

//*****************************************************************************************************
iimage_2d_ptr_t texture_2d::get_image_2d( void_t ) 
{
    return _image_ptr ;
}

//*****************************************************************************************************
image_type texture_2d::get_image_type( void_t ) const 
{
    so_assert( _image_ptr != nullptr ) ;
    return _image_ptr->get_type() ;
}

//*****************************************************************************************************
pixel_type texture_2d::get_pixel_type( void_t ) const 
{
    so_assert( _image_ptr != nullptr ) ;
    return _image_ptr->get_pixel_type() ;
}

//*****************************************************************************************************
so_gpu::image_ptr_t texture_2d::get_image( void_t ) 
{
    return _image_ptr ;
}

//*****************************************************************************************************
so_gpu::image_cptr_t texture_2d::get_image( void_t ) const 
{
    return _image_ptr ;
}

//*****************************************************************************************************
void_t texture_2d::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

