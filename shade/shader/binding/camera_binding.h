//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SHADER_BINDING_CAMERA_BINDING_H_
#define _SNAKEOIL_SHADE_SHADER_BINDING_CAMERA_BINDING_H_

#include "../../variable.h"

namespace so_shade
{
    namespace so_shader
    {
        enum class camera_binding_point
        {
            undefined,
            camera_frame,
            camera_matrix,
            view_matrix,
            proj_matrix,
            mvp_matrix,
            proj_info_0 // vec4( near, far, -, - )
        };

        struct camera_binding
        {
            so_this_typedefs( camera_binding ) ;

        public:

            so_shade::variable_t var ;
            camera_binding_point binding_point ;

        public:

            camera_binding( void_t ) {}
            camera_binding( this_cref_t rhv ) : var(rhv.var), 
                binding_point( rhv.binding_point )
            {}

            this_ref_t operator = ( this_cref_t rhv )
            {
                var = rhv.var ;
                binding_point = rhv.binding_point ;
                return *this ;
            }

        };
        so_typedef( camera_binding ) ;
    }
}

#endif
