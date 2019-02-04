//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_IMAGE_DEPTH_IMAGE_2D_HPP_
#define _SNAKEOIL_GPU_IMAGE_DEPTH_IMAGE_2D_HPP_

#include "iimage_2d.h"
#include "../enums/color_types.h"
#include "../enums/type_traits.h"

#include "../converter/convert_core_gfx_type.h"
#include "../converter/convert_type_to_pixel_type.h"
#include "../converter/convert_type_to_image_format.h"
#include "../buffer/buffer.hpp"


namespace so_gpu
{
    template< typename depth_t >
    class depth_image_2d : public iimage_2d
    {
        so_this_typedefs( depth_image_2d<depth_t> ) ;        
        so_typedefs( so_gpu::mem_buffer<depth_t>, depths ) ;
        
        typedef depth_t const & depth_cref_t ;

    private:

        depths_t _depths ;

        size_t _width = 0 ;
        size_t _height = 0 ;

    public:

        static this_ptr_t create( void_t )
        {
            return this_t::create( this_t(), "[so_gpu::depth_image_2d::create]" ) ;
        }

        static this_ptr_t create( so_memory::purpose_cref_t p )
        {
            return this_t::create( this_t(), p ) ;
        }

        static this_ptr_t create( this_rref_t rhv, std::string const & purpose )
        {
            return so_gpu::memory::alloc( std::move( rhv ), purpose ) ;
        }

        static this_ptr_t destroy( this_ptr_t ptr )
        {
            return so_gpu::memory::dealloc( ptr ) ;
        }

    public:

        static image_type get_image_type( void_t )
        {
            return so_gpu::image_type::depth ;
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

        depth_image_2d( void_t )
        {
            image::set_type( so_gpu::image_type::depth ) ;
           
            pixel_type pt = so_gpu::convert_type_to_pixel_type( so_gpu::type_traits< depth_t >::gfx_type ) ;
            //image_format imf = so_gpu::convert_type_to_image_format( so_gpu::type_traits< depth_t >::gfx_type_struct ) ;

            image::set_pixel_type( pt ) ;
            image::set_format( so_gpu::image_format::depth_component ) ;

            auto t = so_gpu::convert_from(so_gpu::type_traits< depth_t >::gfx_type) ;
            auto ts = so_gpu::convert_from(so_gpu::type_traits< depth_t >::gfx_type_struct) ;

            _depths.add_layout_element(
                so_core::type_traits< depth_t >::core_type,
                so_core::type_traits< depth_t >::core_type_structure
                ) ;
        }

        depth_image_2d( size_t width, size_t height )
        {
            image::set_type( so_gpu::image_type::depth ) ;
         

            pixel_type pt = so_gpu::convert_type_to_pixel_type( so_gpu::type_traits< depth_t >::gfx_type ) ;
            //image_format imf = so_gpu::convert_type_to_image_format( so_gpu::type_traits< depth_t >::gfx_type_struct ) ;

            image::set_pixel_type( pt ) ;
            image::set_format( so_gpu::image_format::depth_component ) ;

            _depths.add_layout_element(
                so_core::type_traits< depth_t >::core_type,
                so_core::type_traits< depth_t >::core_type_structure
                ) ;
            
            _width = width ;
            _height = height ;
            _depths.resize( width * height ) ;
        }

        depth_image_2d( this_t && rhv ) : iimage_2d( std::move(rhv) )
        {
            _depths = std::move( rhv._depths ) ;
            _width = rhv._width ;
            _height = rhv._height ;
        }

        virtual void_t resize( size_t width, size_t height )
        {
            _width = width ; _height = height ;
            _depths.resize( width * height ) ;
        }

        void_t set_pixel( size_t i, depth_cref_t pixel )
        {
            so_assert( i < _depths.size() ) ;
            _depths[i] = pixel ;
        }

        void_t set_pixel( size_t x, size_t y, depth_cref_t pixel )
        {
            size_t index = y * _width + x ;
            so_assert( index < _depths.size() ) ;
            _depths[index] = pixel ;
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
            return _width * _height * sizeof(depth_t) ;
        }

        virtual void_cptr_t get_raw_pointer( void_t ) const 
        {
            return _depths.get_raw_pointer() ;
        }

        virtual void_ptr_t get_raw_pointer( void_t )
        {
            return _depths.get_raw_pointer() ;
        }

        virtual void_t destroy( void_t )
        {
            this_t::destroy( this ) ;
        }
        
    };
}

#endif

