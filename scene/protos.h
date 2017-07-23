//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_PROTOS_H_
#define _SNAKEOIL_SCENE_PROTOS_H_

#include <snakeoil/core/macros/typedef.h>

namespace so_scene
{
    namespace so_node
    {
        so_class_proto_typedefs( iparent ) ;
        
        so_class_proto_typedefs( node ) ;
        so_class_proto_typedefs( group ) ;
        so_class_proto_typedefs( decorator ) ;
        so_class_proto_typedefs( leaf ) ;
        so_class_proto_typedefs( logic_group ) ;
        so_class_proto_typedefs( logic_decorator ) ;

        so_class_proto_typedefs( transform_3d ) ;
        so_class_proto_typedefs( renderable ) ;
        so_class_proto_typedefs( render_state ) ;
        so_class_proto_typedefs( camera ) ;

    }

    namespace so_visitor
    {
        so_class_proto_typedefs( ivisitor ) ;
    }
}
    
#endif

