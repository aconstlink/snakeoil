//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_DAE_DAE_DOCUMENT_H_
#define _SNAKEOIL_IMEX_MODULE_DAE_DAE_DOCUMENT_H_

#include "dae_protos.h"
#include "dae_node.h"
#include "dae_visitor.h"

namespace so_imex
{
    namespace so_dae
    {
        struct document
        {
            so_dae::collada_ptr_t collada_node ;

            // COLLADA
            // asset

            // library_effects
            // library_materials
            // library_geometries
            // visual_scene
            // scene

            document( void_t )
            {}


            ~document( void_t )
            {
                if( so_core::is_not_nullptr( collada_node ) )
                {
                    so_imex::memory::dealloc( collada_node ) ;
                }
            }
        };
    }
}

#endif