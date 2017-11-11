//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include <snakeoil/core/macros/typedef.h>

namespace so_ui
{
    so_class_proto_typedefs( inode_listener ) ;

    namespace so_node
    {
        so_class_proto_typedefs( iparent ) ;

        so_class_proto_typedefs( node ) ;
        so_class_proto_typedefs( group ) ;
        so_class_proto_typedefs( decorator ) ;
        so_class_proto_typedefs( leaf ) ;
        so_class_proto_typedefs( logic_group ) ;
        so_class_proto_typedefs( logic_decorator ) ;
        so_class_proto_typedefs( layer_2d ) ;

        so_class_proto_typedefs( transform_2d ) ;
        so_class_proto_typedefs( renderable ) ;
        so_class_proto_typedefs( render_state ) ;

        so_class_proto_typedefs( area_2d ) ;
        so_class_proto_typedefs( row_layout ) ;
        so_class_proto_typedefs( column_layout ) ;

        so_class_proto_typedefs( input_event ) ;

        so_class_proto_typedefs( fill_area_2d ) ;
    }

    namespace so_visitor
    {
        so_class_proto_typedefs( ivisitor ) ;

        so_class_proto_typedefs( rect_render_2d ) ;
        so_class_proto_typedefs( image_render_2d ) ;
        so_class_proto_typedefs( text_render_2d ) ;
        so_class_proto_typedefs( line_render_2d ) ;
    }
}

