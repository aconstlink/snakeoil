//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_MESH_ASSET_H_
#define _SNAKEOIL_IMEX_NODE_MESH_ASSET_H_

#include "../leaf.h"
#include "../../manager/mesh_manager.h"

#include <snakeoil/geometry/protos.h>

namespace so_imex
{
    namespace so_node
    {
        class SNAKEOIL_IMEX_API mesh_asset : public so_imex::so_node::leaf
        {
            so_this_typedefs( mesh_asset ) ;
            so_typedefs( mesh_manager::handle_t, handle ) ;

        private:

            handle_t _hnd ;

        private:

            // non-copyable
            mesh_asset( this_cref_t ){}

        public:

            mesh_asset( void_t ) ;
            mesh_asset( this_rref_t ) ;
            mesh_asset( handle_rref_t ) ;
            virtual ~mesh_asset( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            so_geo::imesh_cptr_t get_mesh( void_t ) const ;
            so_imex::key_cref_t get_key( void_t ) const ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( mesh_asset ) ;
    }
}

#endif
