//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_NULL_NULL_API_H_
#define _SNAKEOIL_GPU_API_NULL_NULL_API_H_

#include "inull_api.h"

namespace so_gpu
{
    namespace so_null
    {
        class SNAKEOIL_GPU_API null_api : public inull_api
        {
            so_this_typedefs( null_api ) ;

        public:

            null_api( void_t ) ;
            null_api( this_rref_t ) ;
            null_api( this_cref_t ) = delete ;
            virtual ~null_api( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public: // interface

            virtual api_type get_type( void_t ) const ;
            virtual void_t destroy( void_t ) ;
        };
        so_typedef( null_api ) ;
    }
}

#endif