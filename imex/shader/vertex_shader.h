//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_SHADER_VERTEX_SHADER_H_
#define _SNAKEOIL_IMEX_SHADER_VERTEX_SHADER_H_

#include "shader.h"

#include <snakeoil/shade/shader/binding/geometry_binding.h>

namespace so_imex
{
    struct vertex_shader
    {
        so_this_typedefs( vertex_shader ) ;

        so_typedefs( so_std::vector<so_shade::so_shader::texcoord_binding>,
            texcoord_bindings ) ;       

    public:

        so_shade::so_shader::position_binding_t position_binding ;
        so_shade::so_shader::normal_binding_t normal_binding ;

        texcoord_bindings_t texcoord_bindings ;
        
        so_imex::shader shader ;

    public:

        vertex_shader( so_imex::shader_cref_t shd ) : shader(shd) {}
        vertex_shader( this_rref_t rhv )
        {
            *this = std::move( rhv ) ;
        }

        this_ref_t operator = ( this_rref_t rhv )
        {
            position_binding = std::move( rhv.position_binding ) ;
            normal_binding = std::move( rhv.normal_binding ) ;
            texcoord_bindings = std::move( rhv.texcoord_bindings ) ;
            
            shader = std::move( rhv.shader ) ;
            
            return *this ;
        }
    };
    so_typedef( vertex_shader ) ;
}

#endif
