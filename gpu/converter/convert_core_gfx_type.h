//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_CONVERT_CORE_GFX_TYPE_H_
#define _SNAKEOIL_GPU_CONVERT_CORE_GFX_TYPE_H_

#include "../enums/types.h"
#include <snakeoil/core/enums/types.h>

namespace so_gpu
{
    namespace so_internal
    {
        static const so_gpu::type gfx_type_from_core_type_array[] =
        {
            so_gpu::type::undefined,
            so_gpu::type::tchar,
            so_gpu::type::tuchar,
            so_gpu::type::tshort,
            so_gpu::type::tushort,
            so_gpu::type::tint,
            so_gpu::type::tuint,
            so_gpu::type::tfloat,
            so_gpu::type::tdouble,
            so_gpu::type::tbool
        } ;

        static const size_t gfx_type_from_core_type_array_size = 
            sizeof(gfx_type_from_core_type_array)/sizeof(so_gpu::type) ;

        static const so_core::type core_type_from_gfx_type_array[] =
        {
            so_core::type::undefined,
            so_core::type::tchar,
            so_core::type::tuchar,
            so_core::type::tshort,
            so_core::type::tushort,
            so_core::type::tint,
            so_core::type::tuint,
            so_core::type::tfloat,
            so_core::type::tdouble,
            so_core::type::tbool
        } ;

        static const  size_t core_type_from_gfx_type_array_size = 
            sizeof(core_type_from_gfx_type_array)/sizeof(so_core::type) ;

        static const so_gpu::type_struct gfx_type_struct_from_core_type_struct_array[] =
        {
            so_gpu::type_struct::undefined,
            so_gpu::type_struct::vec1,
            so_gpu::type_struct::vec2,
            so_gpu::type_struct::vec3,
            so_gpu::type_struct::vec4,
            so_gpu::type_struct::mat2,
            so_gpu::type_struct::mat3,
            so_gpu::type_struct::mat4
        } ;

        static const  size_t gfx_type_struct_from_core_type_struct_array_size = 
            sizeof(gfx_type_struct_from_core_type_struct_array)/sizeof(so_gpu::type_struct) ;

        static const so_core::type_structure core_type_struct_from_gfx_type_struct_array[] =
        {
            so_core::type_structure::undefined,
            so_core::type_structure::vec1,
            so_core::type_structure::vec2,
            so_core::type_structure::vec3,
            so_core::type_structure::vec4,
            so_core::type_structure::mat2,
            so_core::type_structure::mat3,
            so_core::type_structure::mat4
        } ;

        static const  size_t core_type_struct_from_gfx_type_size = sizeof(core_type_struct_from_gfx_type_struct_array)/sizeof(so_core::type_structure) ;
    }

    static so_gpu::type convert_from( so_core::type t ) 
    {
        return size_t(t) >= so_gpu::so_internal::gfx_type_from_core_type_array_size ? so_gpu::so_internal::gfx_type_from_core_type_array[0] : so_gpu::so_internal::gfx_type_from_core_type_array[size_t(t)] ;
    }

    static so_core::type convert_from( so_gpu::type t ) 
    {
        return size_t(t) >= so_gpu::so_internal::core_type_from_gfx_type_array_size ? so_gpu::so_internal::core_type_from_gfx_type_array[0] : so_gpu::so_internal::core_type_from_gfx_type_array[size_t(t)] ;
    }

    static so_gpu::type_struct convert_from( so_core::type_structure ts ) 
    {
        return size_t(ts) >= so_gpu::so_internal::gfx_type_struct_from_core_type_struct_array_size ? so_gpu::so_internal::gfx_type_struct_from_core_type_struct_array[0] : so_gpu::so_internal::gfx_type_struct_from_core_type_struct_array[size_t(ts)] ;
    }

    static so_core::type_structure convert_from( so_gpu::type_struct ts ) 
    {
        return size_t(ts) >= so_gpu::so_internal::core_type_struct_from_gfx_type_size ? so_gpu::so_internal::core_type_struct_from_gfx_type_struct_array[0] : so_gpu::so_internal::core_type_struct_from_gfx_type_struct_array[size_t(ts)] ;
    }
}

#endif

