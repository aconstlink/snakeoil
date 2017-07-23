//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_VIEWPORT_VIEWPORT_2D_H_
#define _SNAKEOIL_GPU_VIEWPORT_VIEWPORT_2D_H_

#include "../typedefs.h"

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector4.hpp>
#include <snakeoil/math/utility/2d/viewport.hpp>

namespace so_gpu
{
    class viewport_2d
    {
        typedef so_math::vector4< size_t > xywh_t ;

    private:

        xywh_t _xywh ;

    public:

        viewport_2d( void_t )
        {
            set( 0, 0, 0, 0 ) ;
        }

        viewport_2d( size_t n )
        {
            set( n, n, n, n ) ;
        }

        viewport_2d( size_t start_x, size_t start_y, size_t width, size_t height )
        {
            set( start_x, start_y, width, height ) ;
        }
        
        viewport_2d( so_math::so_2d::viewport const & vp )
        {
            set( vp.get_x(), vp.get_y(), vp.get_width(), vp.get_height() ) ;
        }

        viewport_2d( xywh_t const & xywh )
        {
            set( xywh.x(), xywh.y(), xywh.z(), xywh.w() ) ;
        }

        void set( size_t start_x, size_t start_y, size_t width, size_t height )
        {
            _xywh = xywh_t( start_x, start_y, width, height ) ;
        }

    public:

        size_t get_x( void_t ) const{ return _xywh.x() ; }
        size_t get_y( void_t ) const{ return _xywh.y() ; }
        size_t get_width( void_t ) const { return _xywh.z() ; }
        size_t get_height( void_t ) const { return _xywh.w() ; }

        template< typename type_t >
        type_t get_width( void_t ) const { return static_cast<type_t>(_xywh.z()) ; }
        
        template< typename type_t >
        type_t get_height( void_t ) const { return static_cast<type_t>(_xywh.w()) ; }

        template< typename type_t >
        so_math::vector2< type_t > get_width_height( void_t ) const 
        { return so_math::vector2< type_t >( type_t(_xywh.z()), type_t(_xywh.w()) ) ; }

        template< typename type_t >
        so_math::vector2< type_t > get_xy( void_t ) const 
        { return so_math::vector2< type_t >( type_t(_xywh.x()), type_t(_xywh.y()) ) ; }

        template< typename type_t >
        so_math::vector4< type_t > get_as_vec4( void_t ) const 
        { return so_math::vector4< type_t >( _xywh ) ; }
    } ;

    typedef viewport_2d const & viewport_2d_cref_t ;
} 

#endif

