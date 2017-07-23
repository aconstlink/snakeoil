//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_KERNEL_MATH_KERNEL_H_
#define _SNAKEOIL_SHADE_KERNEL_MATH_KERNEL_H_

#include "kernel.h"

namespace so_shade
{
    namespace so_kernel
    {
        class SNAKEOIL_SHADE_API cos : public kernel
        {
            so_this_typedefs( cos );

        public:

            cos( void_t ) ;
            cos( this_rref_t rhv ) ;
            virtual ~cos( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            virtual void_t destroy( void_t ) ;

        public:

            virtual size_t get_num_inputs( void_t ) const ;
            virtual size_t get_num_outputs( void_t ) const ;
            virtual bool_t validate_input_types( types_cref_t inputs ) const ;
            virtual types_t determine_output_types( types_cref_t types_in ) const ;

        };
        so_typedef( cos );

        class SNAKEOIL_SHADE_API multiply : public kernel
        {
            so_this_typedefs( multiply );

        public:

            multiply( void_t ) ;
            multiply( this_rref_t rhv ) ;
            virtual ~multiply( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            virtual void_t destroy( void_t ) ;

        public:

            virtual size_t get_num_inputs( void_t ) const ;

            virtual size_t get_num_outputs( void_t ) const ;

            virtual bool_t validate_input_types( types_cref_t ) const ;

            virtual types_t determine_output_types( types_cref_t types_in ) const; 

        private:

            static so_shade::type_base same_type_base_only( 
                so_shade::type_base arg0, so_shade::type_base arg1 ) ;
        };
        so_typedef( multiply );


        class SNAKEOIL_SHADE_API dot : public kernel
        {
            so_this_typedefs( dot );

        public:

            dot( void_t ) ;
            dot( this_rref_t rhv ) ;
            virtual ~dot( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t p ) ;
            virtual void_t destroy( void_t ) ;

        public:

            virtual size_t get_num_inputs( void_t ) const ;
            virtual size_t get_num_outputs( void_t ) const ;
            virtual bool_t validate_input_types( types_cref_t types_in ) const ;
            virtual types_t determine_output_types( types_cref_t types_in ) const ;
            static so_shade::type_base same_type_base_only( 
                so_shade::type_base arg0, so_shade::type_base arg1 ) ;
        };
        so_typedef( dot ) ;
    }
}

#endif
