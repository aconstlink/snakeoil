//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_SHADER_VARIABLE_TYPES_H_
#define _SNAKEOIL_GPU_SHADER_VARIABLE_TYPES_H_

#include "../typedefs.h"
#include "types.h"

#include <string>

namespace so_gpu
{
    enum class shader_variable_type
    {
        undefined,
        vec1f,
        vec2f,
        vec3f,
        vec4f,
        vec1i,
        vec2i,
        vec3i,
        vec4i,
        vec1ui,
        vec2ui,
        vec3ui,
        vec4ui,
        vec1us,
        vec2us,
        vec3us,
        vec4us,
        vec1b,
        vec2b,
        vec3b,
        vec4b,
        mat2f,
        mat3f,
        mat4f,
        sampler_1d,
        sampler_2d,
        sampler_3d,
        sampler_1i,
        sampler_2i,
        sampler_3i,
        sampler_1ui,
        sampler_2ui,
        sampler_3ui,
        sampler_cube,
        sampler_1d_shadow,
        sampler_2d_shadow,
        sampler_buffer,
        sampler_buffer_i,
        sampler_buffer_ui,
        sampler_2d_array,
        sampler_2d_array_i,
        sampler_2d_array_ui,
        sampler_unknown,
        num_shader_variable_types
    } ;

    namespace so_internal
    {
        static const size_t shader_variable_type_element_count_array[] = {
            0, // svt_undefined
            1, // svt_float
            2, // svt_float_vec2
            3, // svt_float_vec3
            4, // svt_float_vec4
            1, // svt_int
            2, // svt_int_vec2
            3, // svt_int_vec3
            4, // svt_int_vec4
            1, // svt_uint
            2, // svt_uint_vec2
            3, // svt_uint_vec3
            4, // svt_uint_vec4
            1, // svt_ushort
            2, // svt_ushort_vec2
            3, // svt_ushort_vec3
            4, // svt_ushort_vec4
            1, // svt_bool
            2, // svt_bool_vec2
            3, // svt_bool_vec3
            4, // svt_bool_vec4
            4, // svt_float_mat2
            9, // svt_float_mat3
            16, // svt_float_mat4
            1, // svt_sampler_1d
            1, // svt_sampler_2d
            1, // svt_sampler_3d
            1, // svt_sampler_1i
            1, // svt_sampler_2i
            1, // svt_sampler_3i
            1, // svt_sampler_1ui
            1, // svt_sampler_2ui
            1, // svt_sampler_3ui
            1, // svt_sampler_cube
            1, // svt_sampler_1d_shadow
            1, // svt_sampler_2d_shadow
            1, // svt_sampler_buffer
            1, // svt_sampler_buffer_i
            1, // svt_sampler_buffer_ui
            1, // svt_sampler_2d_array
            1, // svt_sampler_2d_array_i
            1, // svt_sampler_2d_array_ui
            1 // sampler_unknown
        } ;

        static const size_t shader_variable_type_size_array[] = {
            0,
            sizeof (float),
            sizeof (float)*2,
            sizeof (float)*3,
            sizeof (float)*4,
            sizeof (int),
            sizeof (int)*2,
            sizeof (int)*3,
            sizeof (int)*4,
            sizeof (unsigned int),
            sizeof (unsigned int)*2,
            sizeof (unsigned int)*3,
            sizeof (unsigned int)*4,
            sizeof ( unsigned short ),
            sizeof ( unsigned short ) * 2,
            sizeof ( unsigned short ) * 3,
            sizeof ( unsigned short ) * 4,
            sizeof (bool),
            sizeof (bool)*2,
            sizeof (bool)*3,
            sizeof (bool)*4,
            sizeof (float)*4,
            sizeof (float)*9,
            sizeof (float)*16,
            sizeof (uint_t), // sampler1d
            sizeof (uint_t), // sampler2d
            sizeof (uint_t), // sampler3d
            sizeof (uint_t), // sampler1i
            sizeof (uint_t), // sampler2i
            sizeof (uint_t), // sampler3i
            sizeof (uint_t), // sampler1ui
            sizeof (uint_t), // sampler2ui
            sizeof (uint_t), // sampler3ui
            sizeof (uint_t), // samplercube
            sizeof (uint_t), // sampler1d shadow
            sizeof (uint_t), // sampler2d shadow
            sizeof (uint_t), // sampler buffer
            sizeof (uint_t),
            sizeof (uint_t),
            sizeof (uint_t), // sampler 2d array
            sizeof (uint_t), // sampler 2d array i
            sizeof (uint_t), // sampler 2d array ui
            sizeof (uint_t) // sampler_unknown
        } ;

        static const std::string shader_variable_type_string_array[] = {
            "undefined", "vec1f", "vec2f", "vec3f",
            "vec4f", "vec1i", "vec2i", "vec3i", "vec4i",
            "uint", "vec2ui", "vec3ui", "vec4ui",
            "ushort", "vec2us", "vec3us", "vec4us",
            "vec1b", "vec2b", "vec3b", "vec4b", "mat2f",
            "mat3f", "mat4f", "sampler_1d", "sampler_2d", "sampler_3d",
            "sampler_1i", "sampler_2i", "sampler_3i", "sampler_1ui", "sampler_2ui", "sampler_3ui",
            "sampler_cube", "sampler_1d_shadow", "sampler_2d_shadow",
            "sampler_buffer", "sampler_buffer_i", "sampler_buffer_ui",
            "sampler_2d_array", "sampler_2d_array_i", "sampler_2d_array_ui",
            "sampler_unknown"
        } ;
    }

    static std::string const & to_string( shader_variable_type svt )
    {
        return so_internal::shader_variable_type_string_array[uint_t(svt)] ;
    }

    static size_t size_of( shader_variable_type t )
    {
        return so_internal::shader_variable_type_size_array[uint_t(t)] ;
    }


    /// Returns the element count of the given shader variable.

    static size_t element_count_of( shader_variable_type svt )
    {
        return so_internal::shader_variable_type_element_count_array[uint_t(svt)] ;
    }

    /// returns true if the passed type is a sampler type.
    static bool_t is_sampler( shader_variable_type svt )
    {
        switch( svt )
        {
        case shader_variable_type::sampler_1d : return true ;
        case shader_variable_type::sampler_2d : return true ;
        case shader_variable_type::sampler_3d : return true ;
        case shader_variable_type::sampler_1i : return true ;
        case shader_variable_type::sampler_2i : return true ;
        case shader_variable_type::sampler_3i : return true ;
        case shader_variable_type::sampler_1ui : return true ;
        case shader_variable_type::sampler_2ui : return true ;
        case shader_variable_type::sampler_3ui : return true ;
        case shader_variable_type::sampler_cube : return true ;
        case shader_variable_type::sampler_1d_shadow : return true ;
        case shader_variable_type::sampler_2d_shadow : return true ;
        case shader_variable_type::sampler_buffer : return true ;
        case shader_variable_type::sampler_buffer_i : return true ;
        case shader_variable_type::sampler_buffer_ui : return true ;
        case shader_variable_type::sampler_2d_array : return true ;
        case shader_variable_type::sampler_2d_array_i : return true ;
        case shader_variable_type::sampler_2d_array_ui : return true ;
        case shader_variable_type::sampler_unknown : return true ;
        default: return false ;
        }
        return false ;
    }

    static shader_variable_type from_type_to_data_buffer_shadar_type( so_gpu::type t )
    {
        switch( t )
        {
        case so_gpu::type::tfloat: return shader_variable_type::sampler_buffer ;
        case so_gpu::type::tint: return shader_variable_type::sampler_buffer_i ;
        case so_gpu::type::tuint: return shader_variable_type::sampler_buffer_ui ;
        default: return shader_variable_type::undefined ;
        }
        return shader_variable_type::undefined ;
    }
}

#endif

