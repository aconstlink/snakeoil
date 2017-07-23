//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_SHADER_SHADER_TYPES_H_
#define _SNAKEOIL_GPU_SHADER_SHADER_TYPES_H_

namespace so_gpu
{
    /// depicts the shader types supported by the engine.
    /// does not necessarily mean, they are supported by
    /// the underly graphics api.
    enum class shader_type
    {
        undefined,
        vertex_shader,
        geometry_shader,
        pixel_shader
    };

    // others required
    // control shader
    // evaluation shader
    // compute shader
}

#endif

