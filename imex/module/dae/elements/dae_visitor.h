//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_DAE_ELEMENTS_DAE_VISITOR_H_
#define _SNAKEOIL_IMEX_MODULE_DAE_ELEMENTS_DAE_VISITOR_H_

#include "dae_protos.h"

#include <snakeoil/log/global.h>

namespace so_imex
{
    namespace so_dae
    {
        class visitor
        {
        public:

            virtual void_t visit( so_dae::collada_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::asset_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::unit_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::geometry_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::mesh_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::source_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::float_array_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::technique_common_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::accessor_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::param_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::vertices_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::input_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::polygons_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::primitives_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::data_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::library_effects_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::library_materials_ptr_t ) = 0 ;
            virtual void_t visit( so_dae::library_geometries_ptr_t ) = 0 ;

        public:

            virtual void_t descent( so_dae::node_ptr_t ) = 0 ;
            virtual void_t ascent( so_dae::node_ptr_t ) = 0 ;
        };
        so_typedef( visitor ) ;
    }
}

#endif