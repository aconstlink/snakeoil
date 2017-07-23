//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_PROTOS_H_
#define _SNAKEOIL_IMEX_PROTOS_H_

#include <snakeoil/core/macros/typedef.h>

namespace so_imex
{
    so_class_proto_typedefs( iscene_module ) ;
    so_class_proto_typedefs( iimage_module ) ;
    so_class_proto_typedefs( imesh_module ) ;
    so_class_proto_typedefs( icode_module ) ;

    so_class_proto_typedefs( code_manager ) ;
    so_class_proto_typedefs( mesh_manager ) ;
    so_class_proto_typedefs( image_manager ) ;
    so_class_proto_typedefs( graph_manager ) ;
    so_class_proto_typedefs( shader_manager ) ;
    so_class_proto_typedefs( camera_manager ) ;

    so_class_proto_typedefs( icamera ) ;
    so_class_proto_typedefs( generic_camera ) ;
    so_class_proto_typedefs( ilens ) ;
    so_class_proto_typedefs( pinhole_lens ) ;

    so_class_proto_typedefs( pinhole_camera ) ;

    so_class_proto_typedefs( iimage ) ;
    so_class_proto_typedefs( image ) ;
    so_class_proto_typedefs( image_in_file) ;

    so_class_proto_typedefs( isystem ) ;
    so_class_proto_typedefs( system ) ;

    namespace so_node
    {
        so_class_proto_typedefs( node ) ;
        so_class_proto_typedefs( group ) ;
        so_class_proto_typedefs( decorator ) ;
        so_class_proto_typedefs( leaf ) ;
        so_class_proto_typedefs( logic_group ) ;

        
        so_class_proto_typedefs( imported_from ) ;
        
        so_class_proto_typedefs( mesh ) ;
        so_class_proto_typedefs( mesh_asset ) ;

        so_class_proto_typedefs( material ) ;
        so_class_proto_typedefs( material_state ) ;
        so_class_proto_typedefs( render_states ) ;

        so_class_proto_typedefs( image_asset ) ;

        so_class_proto_typedefs( shader ) ;
        so_class_proto_typedefs( shader_asset) ;

        so_class_proto_typedefs( camera_asset ) ;
    }

    namespace so_db
    {
        so_class_proto_typedefs( iimage_database ) ;
        so_class_proto_typedefs( igraph_database ) ;
        so_class_proto_typedefs( icode_database ) ;
        so_class_proto_typedefs( icamera_database ) ;
        so_class_proto_typedefs( igeometry_database ) ;
    }
}

#endif

