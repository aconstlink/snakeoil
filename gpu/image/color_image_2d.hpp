//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_IMAGE_COLOR_IMAGE_2D_HPP_
#define _SNAKEOIL_GPU_IMAGE_COLOR_IMAGE_2D_HPP_

#include "iimage_2d.h"
#include "../enums/color_types.h"
#include "../enums/type_traits.h"
#include "../enums/image_type.h"

#include "../converter/convert_core_gfx_type.h"
#include "../converter/convert_type_to_pixel_type.h"
#include "../converter/convert_type_to_image_format.h"

#include "../buffer/buffer.hpp"

namespace so_gpu
{
    template< typename pixel_t >
    class color_image_2d : public iimage_2d
    {
        so_this_typedefs( color_image_2d<pixel_t> ) ;
                
        typedef so_gpu::allocator<pixel_t> alloc_t ;
        typedef so_gpu::mem_buffer< pixel_t, alloc_t > pixels_t ;
        typedef pixel_t const & pixel_cref_t ;

    private:

        pixels_t _pixels ;

        size_t _width = 0 ;
        size_t _height = 0 ;

    public:

        static this_ptr_t create( void_t ) 
        {
            return this_t::create( this_t(), "[so_gpu::color_image_2d::create]" ) ;
        }

        static this_ptr_t create( so_memory::purpose_cref_t p ) 
        {
            return this_t::create( this_t(), p ) ;
        }

        static this_ptr_t create( this_rref_t rhv, std::string const & purpose ) 
        {
            return so_gpu::memory::alloc( std::move(rhv), purpose ) ;
        }

        static void_t destroy( this_ptr_t ptr ) 
        {
            return so_gpu::memory::dealloc( ptr ) ;
        }

    public:

        static image_type get_image_type( void_t )
        {
            return so_gpu::image_type::color_2d ;
        }

        static pixel_type get_pixel_type( void_t )
        {
            return so_gpu::convert_type_to_pixel_type( so_gpu::type_traits< pixel_t >::gfx_type ) ;
        }

        static image_format get_image_format( void_t )
        {
            return so_gpu::convert_type_to_image_format( so_gpu::type_traits< pixel_t >::gfx_type_struct ) ;
        }

    public:

        color_image_2d( void_t ) : _pixels(alloc_t("[so_gpu] : color_image_2d"))
        {
            image::set_type( so_gpu::image_type::color_2d ) ;
           
            pixel_type pt = so_gpu::convert_type_to_pixel_type( so_gpu::type_traits< pixel_t >::gfx_type ) ;
            image_format imf = so_gpu::convert_type_to_image_format( so_gpu::type_traits< pixel_t >::gfx_type_struct ) ;

            image::set_pixel_type( pt ) ;
            image::set_format( imf ) ;

            //auto t = so_gpu::convert_from(so_gpu::type_traits< pixel_t >::gfx_type) ;
            //auto ts = so_gpu::convert_from(so_gpu::type_traits< pixel_t >::gfx_type_struct) ;

            _pixels.add_layout_element(
                so_core::type_traits< pixel_t >::core_type,
                so_core::type_traits< pixel_t >::core_type_structure
                ) ;

        }

        color_image_2d( size_t width, size_t height ) : _pixels(alloc_t("[so_gpu] : color_image_2d"))
        {
            image::set_type( so_gpu::image_type::color_2d ) ;

            pixel_type pt = so_gpu::convert_type_to_pixel_type( so_gpu::type_traits< pixel_t >::gfx_type ) ;
            image_format imf = so_gpu::convert_type_to_image_format( so_gpu::type_traits< pixel_t >::gfx_type_struct ) ;

            image::set_pixel_type( pt ) ;
            image::set_format( imf ) ;

            _pixels.add_layout_element(
                so_core::type_traits< pixel_t >::core_type,
                so_core::type_traits< pixel_t >::core_type_structure
                ) ;
            
            _width = width ;
            _height = height ;
            _pixels.resize( width * height ) ;
        }

        color_image_2d( this_t && rhv ) : iimage_2d( std::move(rhv) ),
            _pixels( std::move(rhv._pixels), alloc_t("[so_gpu] : color_image_2d" ) )
        {
            _width = rhv._width ;
            _height = rhv._height ;
        }

        virtual ~color_image_2d( void_t ) 
        {

        }

        virtual void_t resize( size_t width, size_t height )
        {
            _width = width ; _height = height ;
            _pixels.resize( width * height ) ;
        }

        void_t set_pixel( size_t i, pixel_cref_t pixel )
        {
            so_assert( i < _pixels.size() ) ;
            _pixels[i] = pixel ;
        }

        void_t set_pixel( size_t x, size_t y, pixel_cref_t pixel )
        {
            size_t index = y * _width + x ;
            so_assert( index < _pixels.size() ) ;
            _pixels[index] = pixel ;
        }

    public: // interface

        virtual size_t get_width( void_t ) const 
        {
            return _width ;
        }

        virtual size_t get_height( void_t ) const 
        {
            return _height ;
        }

        virtual size_t get_sib( void_t ) const 
        {
            return _width * _height * sizeof(pixel_t) ;
        }

        virtual void_cptr_t get_raw_pointer( void_t ) const 
        {
            return _pixels.get_raw_pointer() ;
        }

        virtual void_ptr_t get_raw_pointer( void_t ) 
        {
            return _pixels.get_raw_pointer() ;
        }

        virtual void_t destroy( void_t ) 
        {
            this_t::destroy( this ) ;
        }
        
    };
}

#endif

