//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_SHADER_SHADER_H_
#define _SNAKEOIL_IMEX_SHADER_SHADER_H_

#include "../typedefs.h"
#include "../result.h"
#include "../api.h"
#include "../protos.h"

#include <snakeoil/shade/code/ascii_code.h>

#include <snakeoil/shade/shader/binding/texture_binding.h>
#include <snakeoil/shade/shader/binding/camera_binding.h>
#include <snakeoil/shade/shader/binding/object_binding.h>
#include <snakeoil/shade/shader/binding/buffer_binding.h>

#include <snakeoil/std/container/vector.hpp>

namespace so_imex
{
    struct shader
    {
        so_this_typedefs( shader ) ;

        so_typedefs( so_std::vector<so_shade::so_shader::object_binding>,
            object_bindings ) ;

        so_typedefs( so_std::vector<so_shade::so_shader::camera_binding>,
            camera_bindings ) ;

        so_typedefs( so_std::vector<so_shade::so_shader::texture_binding>,
            texture_bindings ) ;

        so_typedefs( so_std::vector<so_shade::so_shader::buffer_binding>,
            buffer_bindings ) ;

    public:

        object_bindings_t object_bindings ;
        camera_bindings_t camera_bindings ;
        texture_bindings_t texture_bindings ;
        buffer_bindings_t buffer_bindings ;

        so_shade::ascii_code_t code ;

    public:

        shader( void_t ){}
        shader( this_cref_t rhv )
        {
            *this = rhv ;
        }

        shader( this_rref_t rhv )
        {
            *this = std::move( rhv ) ;
        }

        this_ref_t operator = ( this_rref_t rhv )
        {
            object_bindings = std::move( rhv.object_bindings ) ;
            camera_bindings = std::move( rhv.camera_bindings ) ;
            texture_bindings = std::move( rhv.texture_bindings ) ;
            buffer_bindings = std::move( rhv.buffer_bindings ) ;
            code = std::move( rhv.code ) ;
            return *this ;
        }

        this_ref_t operator = ( this_cref_t rhv )
        {
            object_bindings = rhv.object_bindings ;
            camera_bindings = rhv.camera_bindings ;
            texture_bindings = rhv.texture_bindings ;
            buffer_bindings = rhv.buffer_bindings ;
            code = rhv.code ;
            return *this ;
        }
    };
    so_typedef( shader ) ;
}

#endif
