//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_KERNEL_IKERNEL_H_
#define _SNAKEOIL_SHADE_KERNEL_IKERNEL_H_

#include "../result.h"
#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"

#include "../type.h"

#include <snakeoil/std/string/string.hpp>

namespace so_shade
{
    namespace so_kernel
    {
        class SNAKEOIL_SHADE_API ikernel
        { public:

            /// returns the kernel's name
            virtual so_std::string_cref_t get_name( void_t ) const = 0 ;

            /// returns the number of input arguments
            virtual size_t get_num_inputs( void_t ) const = 0 ;

            /// allows to retrieve the name of the input slot id.
            virtual so_std::string_t get_input_name( size_t const ) const = 0 ;

            /// returns the number of output variables
            virtual size_t get_num_outputs( void_t ) const = 0 ;

            /// allows to retrieve the name of the output slot id.
            virtual so_std::string_t get_output_name( size_t const ) const = 0 ;

            /// allows to check if the input types are valid for this kernel
            virtual bool_t validate_input_types( types_cref_t ) const = 0 ;

            /// with the input argument types, the output variable types need
            /// to be determined.
            virtual types_t determine_output_types( types_cref_t ) const = 0 ;

            virtual void_t destroy( void_t ) = 0 ;
        };

        template< typename k >
        k * create_kernel( k && rhv, so_memory::purpose_cref_t p )
        {
            return so_shade::memory::alloc( std::move( rhv ), p );
        }

        template< typename k >
        void_t destroy_kernel( k * ptr )
        {
            so_shade::memory::dealloc( ptr );
        }
    }
}

#endif
