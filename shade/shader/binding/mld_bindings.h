//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_GENERATOR_BINDING_MLD_BINDINGS_H_
#define _SNAKEOIL_SHADE_GENERATOR_BINDING_MLD_BINDINGS_H_

#include "icustom_binding.h"
#include "../../variable.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_shade
{
    namespace so_shader
    {
        /// variable binding points
        enum class mld_binding_point
        {
            undefined,
            mld_info_0,
            mld_info_1,
            mld_info_2
        };

        /// single binding
        struct mld_variable_binding
        {
            so_this_typedefs( mld_variable_binding ) ;

            mld_binding_point binding_point ;
            so_shade::variable_t var ;

        public:

            mld_variable_binding( void_t ) {}
            mld_variable_binding( this_cref_t rhv )
            {
                *this = rhv ;
            }

            mld_variable_binding( this_rref_t rhv )
            {
                *this = std::move(rhv) ;
            }

            this_ref_t operator = ( this_cref_t rhv )
            {
                binding_point = rhv.binding_point ;
                var = rhv.var ;

                return *this ;
            }

            this_ref_t operator = ( this_rref_t rhv )
            {
                binding_point = std::move(rhv.binding_point) ;
                var = std::move(rhv.var) ;

                return *this ;
            }

        };
        so_typedef( mld_variable_binding ) ;

        /// multiple bindings
        struct mld_variable_bindings : public icustom_binding
        {
            so_this_typedefs( mld_variable_bindings ) ;
            so_typedefs( so_std::vector<mld_variable_binding>, bindings ) ;

        public:

            bindings_t bindings ;

        public:

            mld_variable_bindings( void_t ) {}
            mld_variable_bindings( this_cref_t rhv )
            {
                bindings = rhv.bindings ;
            }

            mld_variable_bindings( this_rref_t rhv )
            {
                bindings = std::move(rhv.bindings) ;
            }

            virtual ~mld_variable_bindings( void_t ) {}

        public: // statics

            static this_ptr_t create( this_rref_t rhv, so_memory::purpose_cref_t p )
            {
                return so_shade::memory::alloc( std::move( rhv ), p ) ;
            }

            static void_t destroy( this_ptr_t ptr )
            {
                so_shade::memory::dealloc( ptr ) ;
            }

        public:

            virtual void_t destroy( void_t )
            {
                this_t::destroy( this ) ;
            }
        };
        so_typedef( mld_variable_bindings ) ;
    }
}

#endif
