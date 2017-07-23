//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SHADER_SHADER_H_
#define _SNAKEOIL_SHADE_SHADER_SHADER_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"
#include "../result.h"

#include "policy/custom_bindings_policy.h"

namespace so_shade
{
    namespace so_shader
    {
        class SNAKEOIL_SHADE_API shader : private custom_bindings_policy
        {
            so_this_typedefs( shader ) ;
            so_typedefs( custom_bindings_policy, custom_bindings ) ;

        public:

            shader( void_t ) ;
            shader( this_rref_t ) ;
            shader( this_cref_t ) ;

            virtual ~shader( void_t ) ;

        public:

            this_ref_t operator = ( this_cref_t ) ;

        public:

            bool_t add_custom_binding( icustom_binding_ptr_t ) ;
            void_t for_each_custom_binding( custom_bindings_t::for_each_funk_c_t ) const ;

        public: 

            virtual void_t destroy( void_t ) = 0 ;
        };
        so_typedef( shader ) ;
    }
}

#endif
