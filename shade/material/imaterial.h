//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_MATERIAL_IMATERIAL_H_
#define _SNAKEOIL_SHADE_MATERIAL_IMATERIAL_H_

#include "../typedefs.h"
#include "../result.h"
#include "../protos.h"
#include "../api.h"

namespace so_shade
{
    namespace so_material
    {
        class SNAKEOIL_SHADE_API imaterial
        {
        public:

            virtual void_t destroy( void_t ) = 0 ;
        };
    }
}

#endif
