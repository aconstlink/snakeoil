//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_ENUM_GEOMETRY_SHADER_OUTPUT_TYPES_H_
#define _SNAKEOIL_GPU_ENUM_GEOMETRY_SHADER_OUTPUT_TYPES_H_

namespace so_gpu
{
    enum class geometry_shader_output_type
    {
        undefined,
        points,
        line_strip,
        triangle_strip
    };    
}

#endif

