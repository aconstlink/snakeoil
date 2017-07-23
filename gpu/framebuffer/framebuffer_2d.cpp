//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "framebuffer_2d.h"

#include <algorithm>

using namespace so_gpu ;

//*****************************************************************************************************
framebuffer_2d::this_ptr_t framebuffer_2d::create( so_memory::purpose_cref_t purpose ) 
{
    return so_gpu::memory::alloc( this_t(), purpose ) ;
}

//*****************************************************************************************************
void_t framebuffer_2d::destroy( this_ptr_t ptr ) 
{
    so_gpu::memory::dealloc( ptr ) ;
}

//*****************************************************************************************************
framebuffer_2d::framebuffer_2d( void_t ) 
{

}

//*****************************************************************************************************
framebuffer_2d::framebuffer_2d( this_rref_t rhv )
{
    _colors = std::move( rhv._colors ) ;
    _depth = rhv._depth ;
    rhv._depth = nullptr ;
}

//*****************************************************************************************************
framebuffer_2d::~framebuffer_2d( void_t ) 
{

}

//*****************************************************************************************************
void_t framebuffer_2d::add_color_target( texture_2d_ptr_t ptr )
{
    auto iter = std::find( _colors.begin(), _colors.end(), ptr ) ;
    if( iter != _colors.end() ) return ;

    _colors.push_back( ptr ) ;
}

//*****************************************************************************************************
void_t framebuffer_2d::set_depth_target( depth_texture_2d_ptr_t ptr )
{
    _depth = ptr ;
}

//*****************************************************************************************************
size_t framebuffer_2d::get_num_color_targets( void_t ) const 
{
    return _colors.size() ;
}

//*****************************************************************************************************
texture_2d_ptr_t framebuffer_2d::get_color_target( size_t index )
{
    if( index >= _colors.size() ) return nullptr ;
    return _colors[index] ;
}

//*****************************************************************************************************
depth_texture_2d_ptr_t framebuffer_2d::get_depth_target( void_t )
{
    return _depth ;
}

//*****************************************************************************************************
framebuffer_2d::color_targets_t & framebuffer_2d::get_color_targets( void_t ) 
{
    return _colors ;
}
//*****************************************************************************************************
void_t framebuffer_2d::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

