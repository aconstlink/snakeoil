//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_CAMERA_CAMERA_ASSET_H_
#define _SNAKEOIL_IMEX_NODE_CAMERA_CAMERA_ASSET_H_

#include "../leaf.h"

#include "../../manager/camera_manager.h"

namespace so_imex
{
    namespace so_node
    {
        class SNAKEOIL_IMEX_API camera_asset : public leaf
        {
            so_this_typedefs( camera_asset ) ;
            so_typedefs( so_imex::camera_manager_t::handle_t, handle ) ;

        private:

            handle_t _hnd ;

        private:

            // non-copyable
            camera_asset( this_cref_t ) {}

        public:

            camera_asset( void_t ) ;
            camera_asset( this_rref_t ) ;
            camera_asset( handle_rref_t ) ;
            virtual ~camera_asset( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            bool_t get_key( so_imex::key_out_t ) const ;
            so_imex::icamera_ptr_t get_camera( void_t ) ;

        public:

            virtual void_t destroy( void_t ) ;

        };
        so_typedef( camera_asset ) ;
    }
}

#endif 
