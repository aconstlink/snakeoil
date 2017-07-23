//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_CONVERTER_CONVERT_IMAGE_TO_SAHDER_VARIABLE_TYPE_H_
#define _SNAKEOIL_GPU_CONVERTER_CONVERT_IMAGE_TO_SAHDER_VARIABLE_TYPE_H_

#include "../enums/shader_variable_types.h"
#include "../enums/image_type.h"
#include "../enums/image_formats.h"
#include "../enums/pixel_type.h"

namespace so_gpu
{
    static so_gpu::shader_variable_type convert_image_to_shadar_type( 
        so_gpu::image_type it, so_gpu::pixel_type pt )
    {
        switch( it )
        {
        case so_gpu::image_type::color_2d:
            switch( pt )
            {
            case pixel_type::tshort: return so_gpu::shader_variable_type::sampler_2d ;
            case pixel_type::tushort: return so_gpu::shader_variable_type::sampler_2d ;
            case pixel_type::tbyte: return so_gpu::shader_variable_type::sampler_2d ;
            case pixel_type::tubyte: return so_gpu::shader_variable_type::sampler_2d ;
            case pixel_type::tuint: return so_gpu::shader_variable_type::sampler_2ui ;
            case pixel_type::tint: return so_gpu::shader_variable_type::sampler_2i ;
            case pixel_type::tfloat: return so_gpu::shader_variable_type::sampler_2d ;
            default: break ;
            }
            break ;
        case so_gpu::image_type::depth:
            switch( pt )
            {
            case pixel_type::tshort: return so_gpu::shader_variable_type::sampler_2d_shadow ;
            case pixel_type::tushort: return so_gpu::shader_variable_type::sampler_2d_shadow ;
            case pixel_type::tbyte: return so_gpu::shader_variable_type::sampler_2d_shadow ;
            case pixel_type::tubyte: return so_gpu::shader_variable_type::sampler_2d_shadow ;
            case pixel_type::tuint: return so_gpu::shader_variable_type::sampler_2d_shadow ;
            case pixel_type::tint: return so_gpu::shader_variable_type::sampler_2d_shadow ;
            case pixel_type::tfloat: return so_gpu::shader_variable_type::sampler_2d_shadow ;
            default: break ;
            }
            break ;

        default: break ;
        }

        return so_gpu::shader_variable_type::undefined ;
    }
}

#endif

