//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_MATERIAL_MATERIAL_H_
#define _SNAKEOIL_SHADE_MATERIAL_MATERIAL_H_

#include "imaterial.h"

namespace so_shade
{
    namespace so_material
    {
        class SNAKEOIL_SHADE_API material : public imaterial
        {
            so_this_typedefs( material ) ;

        private:

            
            
        protected:

        public:

            virtual void_t destroy( void_t ) = 0 ;

        };
    }
}

#endif
