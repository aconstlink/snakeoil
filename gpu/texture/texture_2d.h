//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_TEXTURE_TEXTURE_2D_H_
#define _SNAKEOIL_GPU_TEXTURE_TEXTURE_2D_H_

#include "texture.h"
#include "../image/iimage_2d.h"
#include "../viewport/viewport_2d.h"

#include "../enums/texture_wraps.h"
#include "../enums/texture_filters.h"

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector4.hpp>

namespace so_gpu
{
    class SNAKEOIL_GPU_API texture_2d : public so_gpu::texture
    {
        typedef so_gpu::texture base_t ;
        so_this_typedefs( texture_2d ) ;

        typedef std::pair< texture_wrap_type, texture_wrap_type > wrap_st_t ;
        typedef std::pair< texture_filter_type, texture_filter_type > filter_st_t ;

    private: 

        iimage_2d_ptr_t _image_ptr = nullptr ;

        /// this viewport represents the valid region
        /// within the image.
        so_gpu::viewport_2d _region ;

        /// relative to image coords of region.
        so_math::vec4f_t _tex_coord_info ;

    private: 

        texture_wrap_type _wrap_s = texture_wrap_type::clamp ;
        texture_wrap_type _wrap_t = texture_wrap_type::clamp ;
        
        texture_filter_type _filter_s = texture_filter_type::linear ;
        texture_filter_type _filter_t = texture_filter_type::linear ;        

    public:

        static this_ptr_t create( this_rref_t ) ;
        static this_ptr_t create( this_rref_t, std::string const & purpose ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        texture_2d( iimage_2d_ptr_t ) ;
        texture_2d( this_rref_t rhv ) ;
        virtual ~texture_2d( void_t ) ;

    public:

        so_math::vec2f_t get_tex_dim( void_t ) const ;
        so_math::vec2f_t get_tex_offset( void_t ) const ;
        so_math::vec4f_t get_tex_coord_info( void_t ) const ;

        so_gpu::viewport_2d_cref_t get_region( void_t ) const ;
        void_t set_region( so_gpu::viewport_2d_cref_t ) ;
        
        /// resets the texture's region to the full image dim.
        void_t reset_region( void_t ) ;

    public:

        void_t set_texture_wrap( texture_wrap_type s, texture_wrap_type t ) ;
        void_t set_texture_filter( texture_filter_type s, texture_filter_type t ) ;

        wrap_st_t get_texture_wrap( void_t ) const ;
        filter_st_t get_texture_filter( void_t ) const ;

        iimage_2d_ptr_t get_image_2d( void_t ) ;

    public:

        image_type get_image_type( void_t ) const ;
        pixel_type get_pixel_type( void_t ) const ;    

    public: // interface

        virtual so_gpu::image_ptr_t get_image( void_t ) ;
        virtual so_gpu::image_cptr_t get_image( void_t ) const ;

        virtual void_t destroy( void_t ) ;

    };
    so_typedef( texture_2d ) ;
}

#endif

