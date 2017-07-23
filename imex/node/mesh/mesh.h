//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_GROUP_MESH_H_
#define _SNAKEOIL_IMEX_NODE_GROUP_MESH_H_

#include "../group.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_imex
{
    namespace so_node
    {
        class SNAKEOIL_IMEX_API mesh : public group
        {
            typedef group base_t ;
            so_this_typedefs( mesh ) ;

            struct image_store
            {
                so_std::string_t key ;
                image_asset_ptr_t img_ptr ;
            };
            so_typedef( image_store ) ;
            so_typedefs( so_std::vector<image_store_t>, images ) ;

        private:

            mesh_asset_ptr_t _asset_ptr = nullptr ;
            material_asset_ptr_t _material_ptr = nullptr ;

            images_t _images ;

        public:

            mesh( void_t ) ;
            mesh( this_rref_t ) ;
            virtual ~mesh( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            mesh_asset_ptr_t set_asset( mesh_asset_ptr_t ) ;
            mesh_asset_ptr_t get_asset( void_t ) ;

            bool_t set_material( material_asset_ptr_t ) ;
            material_asset_ptr_t get_material( void_t ) ;

            bool_t add_image( so_std::string_cref_t, so_imex::so_node::image_asset_ptr_t )  ;
            bool_t get_image( so_std::string_cref_t, so_imex::so_node::image_asset_ptr_t & )  ;

        public:

            virtual void_t destroy( void_t ) ;

        };
        so_typedef( mesh ) ;
    }
}

#endif
