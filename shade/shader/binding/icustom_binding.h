//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SHADER_BINDING_ICUSTOM_BINDING_H_
#define _SNAKEOIL_SHADE_SHADER_BINDING_ICUSTOM_BINDING_H_

#include "../../typedefs.h"
#include "../../protos.h"
#include "../../api.h"
#include "../../result.h"

namespace so_shade
{
    namespace so_shader
    {
        struct icustom_binding
        {
            virtual void_t destroy( void_t ) = 0 ;
        };
        so_typedef( icustom_binding ) ;
    }
}

#endif
