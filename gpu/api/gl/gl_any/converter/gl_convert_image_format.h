//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_IMAGE_FORMAT_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_IMAGE_FORMAT_H_

#include "../../../../enums/image_formats.h"
#include "../../../../enums/pixel_type.h"

#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLint image_formats_array_gl[] = 
            {
                0,
                GL_ALPHA,
                //GL_ALPHA4,
                //GL_ALPHA8,
                //GL_ALPHA12,
                //GL_ALPHA16,
                //GL_COMPRESSED_ALPHA,
                //GL_COMPRESSED_LUMINANCE,
                //GL_COMPRESSED_LUMINANCE_ALPHA,
                //GL_COMPRESSED_INTENSITY,
                GL_COMPRESSED_RGB,
                GL_COMPRESSED_RGBA,
                GL_DEPTH_COMPONENT,
                GL_DEPTH_COMPONENT16,
                GL_DEPTH_COMPONENT24,
                GL_DEPTH_COMPONENT32,
                GL_DEPTH24_STENCIL8,
                GL_RED, //GL_LUMINANCE,
                //GL_LUMINANCE4,
                GL_R8, //GL_LUMINANCE8,
                //GL_LUMINANCE12,
                GL_R16, //GL_LUMINANCE16,
                GL_RED, //GL_INTENSITY,
                //GL_INTENSITY4,
                GL_R8, //GL_INTENSITY8,
                //GL_INTENSITY12,
                GL_R16, //GL_INTENSITY16,
                3/*GL_RGB*/,
                GL_RGB4,
                GL_RGB5,
                GL_RGB8,
                GL_RGB10,
                GL_RGB12,
                GL_RGB16,
                GL_RGBA,
                GL_RGBA2,
                GL_RGBA4,
                GL_RGBA8,
                GL_RGBA12,
                GL_RGBA16,
                GL_RGBA32F,
                GL_RGB32F,
                GL_R32F, //GL_ALPHA32F_ARB,
                GL_R32F, //GL_INTENSITY32F_ARB,
                GL_R32F, //GL_LUMINANCE32F_ARB,
                //GL_LUMINANCE_ALPHA32F_ARB,
                GL_RGBA16F,
                GL_RGB16F,
                GL_R16F, //GL_ALPHA16F_ARB,
                GL_R16F, //GL_INTENSITY16F_ARB,
                GL_R16F, //GL_LUMINANCE16F_ARB,
                //GL_LUMINANCE_ALPHA16F_ARB,
                GL_R8I,
                GL_R8UI,
                GL_R32I,
                GL_R32UI,
                GL_RG8I,
                GL_RG8UI,
                GL_RG32I,
                GL_RG32UI,
                GL_RGB8I,
                GL_RGB8UI,
                GL_RGB32I,
                GL_RGB32UI,
                GL_RGBA8I,
                GL_RGBA8UI,
                GL_RGBA32I,
                GL_RGBA32UI
            } ;

            static const GLint check_depth_image_formats_array_gl[] = {
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                GL_DEPTH_COMPONENT,
                GL_DEPTH_COMPONENT16,
                GL_DEPTH_COMPONENT24,
                GL_DEPTH_COMPONENT32,
                GL_DEPTH_STENCIL,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0
            } ;


            static const unsigned int _image_formats_array_size_gl = 
                sizeof(image_formats_array_gl) / sizeof(image_formats_array_gl[0]) ;            
        }

        static GLenum convert_to_gl_pixel_type( so_gpu::pixel_type const pt )
        {
            switch( pt )
            {
                case pixel_type::undefined: return 0 ;
                case pixel_type::tshort: return GL_SHORT ;
                case pixel_type::tushort: return GL_UNSIGNED_SHORT ;
                case pixel_type::tbyte: return GL_BYTE ;
                case pixel_type::tubyte: return GL_UNSIGNED_BYTE ;
                case pixel_type::tuint: return GL_UNSIGNED_INT ;
                case pixel_type::tint: return GL_INT ;
                case pixel_type::tfloat: return GL_FLOAT ;
            }
            return 0 ;
        }

        static GLenum convert_to_gl_pixel_format( image_format const imf )
        {
            switch( imf )
            {
                case image_format::depth_component: return GL_DEPTH_COMPONENT ;
                case image_format::depth24_stencil8:  return GL_DEPTH_STENCIL ;
                case image_format::rgb: return GL_RGB ;
                case image_format::rgba: return GL_RGBA ;
                case image_format::intensity: return GL_RED ;
                default: break ;
            }
            return 0 ;
        }

        static GLenum convert_to_gl_format( image_format const imf, so_gpu::pixel_type const t )
        {

            switch( imf )
            {
            case image_format::depth_component: 
                return GL_DEPTH_COMPONENT ;
            case image_format::depth24_stencil8: 
                return GL_DEPTH24_STENCIL8 ;
            case image_format::rgb: 
                switch( t )
                {
                case pixel_type::tubyte: return GL_RGB ;
                case pixel_type::tint: return GL_RGB32I ;
                case pixel_type::tuint: return GL_RGB32UI ;
                case pixel_type::tfloat: return GL_RGB32F ;
                default: break ;
                }
                break  ;
            case image_format::rgba: 
                switch( t )
                {
                case pixel_type::tubyte: return GL_RGBA ;
                case pixel_type::tint: return GL_RGBA32I ;
                case pixel_type::tuint: return GL_RGBA32UI ;
                case pixel_type::tfloat: return GL_RGBA32F ;
                default: break ;
                }
                break  ;
            case image_format::intensity: 
                switch( t )
                {
                case pixel_type::tubyte: return GL_RED ;
                case pixel_type::tint: return GL_R32I ;
                case pixel_type::tuint: return GL_R32UI ;
                case pixel_type::tfloat: return GL_R32F ;
                default: break ;
                }
                break  ;
                    
            default: break ;
                
            }
            return 0 ;
        }
    }
}

#endif

