//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SHADER_BINDING_BUFFER_BINDING_H_
#define _SNAKEOIL_SHADE_SHADER_BINDING_BUFFER_BINDING_H_

#include "../../variable.h"

namespace so_shade
{
    namespace so_shader
    {
        struct buffer_binding
        {
            so_this_typedefs( buffer_binding ) ;

        public:

            so_shade::buffer_variable_t var ;

        public:

            buffer_binding( void_t ){}
            buffer_binding( this_cref_t rhv ) : var( rhv.var ) {}
            buffer_binding( this_rref_t rhv ) : var(std::move(rhv.var)){}
            ~buffer_binding( void_t ) {}

            this_ref_t operator = ( this_cref_t rhv )
            {
                var = rhv.var ;
                return *this ;
            }

            this_ref_t operator = ( this_rref_t rhv )
            {
                var = std::move( rhv.var ) ;
                return *this ;
            }
        };
        so_typedef( buffer_binding ) ;
    }
}

#endif
