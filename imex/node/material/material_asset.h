//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_MATERIAL_MATERIAL_ASSET_H_
#define _SNAKEOIL_IMEX_NODE_MATERIAL_MATERIAL_ASSET_H_

#include "../leaf.h"
#include "../../manager/shade_manager/material_manager.h"

#include <snakeoil/shade/protos.h>

namespace so_imex
{
    namespace so_node
    {
        class SNAKEOIL_IMEX_API material_asset : public leaf
        {
            so_this_typedefs( material_asset ) ;

            so_typedefs( material_manager_t::handle_t, handle ) ;

        private:

            handle_t _hnd ;

        private:

            

        public:

            material_asset( void_t ) ;
            material_asset( this_rref_t ) ;
            material_asset( handle_rref_t ) ;
            virtual ~material_asset( void_t ) ;

            // non-copyable
            material_asset( this_cref_t ) = delete ;

            this_ref_t operator = ( this_rref_t ) ;
            this_ref_t operator = ( this_cref_t ) = delete ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            so_resource::key_t get_key( void_t ) const ;
            so_shade::so_material::imaterial_ptr_t get_material( void_t ) ;

        public: // interface

            virtual void_t destroy( void_t ) ;

        } ;
        so_typedef( material_asset ) ;
    }
}

#endif
