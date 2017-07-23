//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_ENUM_GEOMETRY_SHADER_INPUT_TYPES_H_
#define _SNAKEOIL_GPU_ENUM_GEOMETRY_SHADER_INPUT_TYPES_H_


namespace so_gpu
{
    enum class geometry_shader_input_type
    {
        undefined,
        points,
        lines,
        lines_adjacency,
        triangles,
        triangles_adjacency
    };
}

#endif

