//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_CONVERT_COLOR_O_TEXTURE_FORMAT_H_
#define _SNAKEOIL_GPU_CONVERT_COLOR_O_TEXTURE_FORMAT_H_

#include "../enums/texture_types.h"
#include "../enums/texture_formats.h"
#include "../enums/color_types.h"

namespace so_gpu
{
    /// used to deduce the texture format for a texture if its image is set.
    static texture_format convert_color_to_texture_format( texture_format tf, color_type ct, type t )
    {
        switch( tf )
        {
        case so_gpu::texture_format::depth_component: return so_gpu::texture_format::depth_component ;
        case so_gpu::texture_format::depth_component16: return so_gpu::texture_format::depth_component16 ;
        case so_gpu::texture_format::depth_component24: return so_gpu::texture_format::depth_component24 ;
        case so_gpu::texture_format::depth_component32: return so_gpu::texture_format::depth_component32 ;
        default: break ;
        }

        texture_format ret_tf = so_gpu::texture_format::undefined ;

        switch( ct )
        {
        case so_gpu::color_type::undefined: break ;

        case so_gpu::color_type::rgb:
        case so_gpu::color_type::bgr:
        {
            switch( t )
            {
            case so_gpu::type::tchar:
            case so_gpu::type::tuchar: ret_tf = so_gpu::texture_format::rgb8 ; break ;
            case so_gpu::type::tfloat: ret_tf = so_gpu::texture_format::rgb32f ; break ;
            case so_gpu::type::tuint: ret_tf = so_gpu::texture_format::rgb32ui ; break ;
            case so_gpu::type::tint: ret_tf = so_gpu::texture_format::rgb32i ; break ;
            default: break ;
            }
        } 
        break ;

        case so_gpu::color_type::rgba:
        case so_gpu::color_type::bgra:
        {
            switch( t )
            {
            case so_gpu::type::tchar:
            case so_gpu::type::tuchar: ret_tf = so_gpu::texture_format::rgba8 ; break ;
            case so_gpu::type::tfloat: ret_tf = so_gpu::texture_format::rgba32f ; break ;
            case so_gpu::type::tuint: ret_tf = so_gpu::texture_format::rgba32ui ; break ;
            case so_gpu::type::tint: ret_tf = so_gpu::texture_format::rgba32i ; break ;
            default: break ;
            }
        } break ;

        case so_gpu::color_type::intensity:
        {
            switch( t )
            {
            case so_gpu::type::tchar:
            case so_gpu::type::tuchar: ret_tf = so_gpu::texture_format::intensity8 ; break ;
            case so_gpu::type::tfloat: ret_tf = so_gpu::texture_format::intensity32f ; break ;
            case so_gpu::type::tuint: ret_tf = so_gpu::texture_format::r32ui ; break ;
            case so_gpu::type::tint: ret_tf = so_gpu::texture_format::r32i ; break ;
            default: break ;
            }
        } break ;
            
        case so_gpu::color_type::luminance:
        {
            switch( t )
            {
            case so_gpu::type::tchar:
            case so_gpu::type::tuchar: ret_tf = so_gpu::texture_format::luminance8 ; break ;
            case so_gpu::type::tfloat: ret_tf = so_gpu::texture_format::luminance32f ; break ;
            case so_gpu::type::tuint: ret_tf = so_gpu::texture_format::r32ui ; break ;
            case so_gpu::type::tint: ret_tf = so_gpu::texture_format::r32i ; break ;
            default: break ;
            }
        } break ;
            
        default: break ;

        }
        return ret_tf ;
    }
}

#endif

