//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SHADER_POLICY_CUSTOM_BINGINDS_POLICY_H
#define _SNAKEOIL_SHADE_SHADER_POLICY_CUSTOM_BINGINDS_POLICY_H

#include "../binding/icustom_binding.h"

#include <snakeoil/std/container/vector.hpp>

#include <functional>

namespace so_shade
{
    namespace so_shader
    {
        class SNAKEOIL_SHADE_API custom_bindings_policy
        {
            so_this_typedefs( custom_bindings_policy ) ;

            so_typedefs( so_std::vector<so_shade::so_shader::icustom_binding_ptr_t>,
                custom_bindings ) ;

        public:

            typedef std::function< bool_t (so_shade::so_shader::icustom_binding_cptr_t) > 
                for_each_funk_c_t ;

        private:

            custom_bindings_t _customs ;

        public:

            custom_bindings_policy( void_t ) ;
            custom_bindings_policy( this_rref_t ) ;
            custom_bindings_policy( this_cref_t ) ;
            ~custom_bindings_policy( void_t ) ;

        public:

            this_ref_t operator = ( this_cref_t ) ;

        public:

            so_shade::result add_binding( icustom_binding_ptr_t ) ;
            void_t for_each( for_each_funk_c_t ) const ;
        };
    }
}

#endif
