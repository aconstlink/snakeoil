//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_KERNEL_KERNEL_H_
#define _SNAKEOIL_SHADE_KERNEL_KERNEL_H_

#include "ikernel.h"

namespace so_shade
{
    namespace so_kernel
    {
        class SNAKEOIL_SHADE_API kernel : public ikernel
        {
            so_this_typedefs( kernel ) ;

        private:

            so_std::string_t _name ;

        public:

            kernel( so_std::string_cref_t name ) : _name(name)
            {}

            kernel( this_rref_t rhv )
            {
                _name = std::move( rhv._name ) ;
            }

            virtual ~kernel( void_t ) 
            {}

        public:

            /// returns the kernel's name
            virtual so_std::string_cref_t get_name( void_t ) const
            {
                return _name ;
            }

            /// allows to retrieve the name of the input slot id.
            virtual so_std::string_t get_input_name( size_t const i ) const 
            {
                return i < this->get_num_inputs() ? 
                    std::move(so_std::string_t("arg_") + std::to_string(i) ) :
                    std::move(so_std::string_t("invalid_") + std::to_string(i) ) ;
            }

            /// allows to retrieve the name of the output slot id.
            virtual so_std::string_t get_output_name( size_t const i ) const
            {
                return i < this->get_num_outputs() ?
                    std::move( so_std::string_t( "out_" ) + std::to_string( i ) ) :
                    std::move( so_std::string_t( "invalid_" ) + std::to_string( i ) ) ;
            }

        public: // not implemented interface
            

            /// returns the number of input arguments
            virtual size_t get_num_inputs( void_t ) const = 0;            

            /// returns the number of output variables
            virtual size_t get_num_outputs( void_t ) const = 0;

            /// allows to check if the input types are valid for this kernel
            virtual bool_t validate_input_types( types_cref_t ) const = 0;

            /// with the input argument types, the output variable types need
            /// to be determined.
            virtual types_t determine_output_types( types_cref_t ) const = 0;

            virtual void_t destroy( void_t ) = 0;

        };
    }
}

#endif
