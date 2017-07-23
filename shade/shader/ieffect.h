//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SHADER_IEFFECT_H_
#define _SNAKEOIL_SHADE_SHADER_IEFFECT_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"
#include "../result.h"

namespace so_shade
{
    namespace so_shader
    {
        class SNAKEOIL_SHADE_API ieffect
        {
        public:

            virtual void_t destroy( void_t ) = 0 ;

        } ;
    } 
}

#endif
