//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_IMAGE_IMAGE_ASSET_H_
#define _SNAKEOIL_IMEX_NODE_IMAGE_IMAGE_ASSET_H_

#include "../leaf.h"

#include "../../manager/image_manager.h"

namespace so_imex
{
    namespace so_node
    {
        class SNAKEOIL_IMEX_API image_asset : public leaf
        {
            so_this_typedefs( image_asset ) ;

            so_typedefs( image_manager::handle_t, handle ) ;

        private:

            handle_t _hnd ;

        private:

            // non-copyable
            image_asset( this_cref_t ) {}

        public:

            image_asset( void_t ) ;
            image_asset( this_rref_t ) ;
            image_asset( handle_rref_t ) ;
            virtual ~image_asset( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            so_resource::key_t get_key( void_t ) const ;
            so_imex::iimage_ptr_t get_image( void_t ) ;

        public: // interface

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( image_asset ) ;
    }
}

#endif
