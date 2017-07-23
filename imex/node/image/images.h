//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_IMAGE_IMAGES_H_
#define _SNAKEOIL_IMEX_NODE_IMAGE_IMAGES_H_

#include "../group.h"

namespace so_imex
{
    namespace so_node
    {
        class SNAKEOIL_IMEX_API images : public group
        {
            typedef group base_t ;
            so_this_typedefs( images ) ;

        private:

            // images here

        public:

            images( void_t ) ;
            images( this_rref_t ) ;
            virtual ~images( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual void_t destroy( void_t ) ;

        };
    }
}

#endif
