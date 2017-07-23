//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_MATERIAL_EFFECT_MATERIAL_H_
#define _SNAKEOIL_SHADE_MATERIAL_EFFECT_MATERIAL_H_

#include "imaterial.h"

namespace so_shade
{
    namespace so_material
    {
        class SNAKEOIL_SHADE_API effect_material : public imaterial
        {
            so_this_typedefs( effect_material ) ;

        private:

            so_shade::so_shader::effect_ptr_t _effect_ptr = nullptr ;

        public:

            effect_material( void_t ) ;
            effect_material( so_shade::so_shader::effect_ptr_t ) ;

            effect_material( this_rref_t ) ;
            effect_material( this_cref_t ) = delete ;
            virtual ~effect_material( void_t ) ;

        public:
           
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            so_shade::so_shader::effect_ptr_t set_effect( so_shade::so_shader::effect_ptr_t ) ;
            so_shade::so_shader::effect_ptr_t get_effect( void_t ) ;

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( effect_material ) ;
    }
}

#endif
