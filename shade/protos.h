//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_PROTOS_H_
#define _SNAKEOIL_SHADE_PROTOS_H_

#include <snakeoil/core/macros/typedef.h>

namespace so_shade
{
    so_class_proto_typedefs( icode ) ;
    so_class_proto_typedefs( ascii_code ) ;

    so_class_proto_typedefs( islot ) ;
    so_class_proto_typedefs( slot_group ) ;
    so_class_proto_typedefs( iinput_slot ) ;
    so_class_proto_typedefs( ioutput_slot ) ;
    so_class_proto_typedefs( input_slot ) ;
    so_class_proto_typedefs( output_slot ) ;
    so_class_proto_typedefs( input_slots ) ;
    so_class_proto_typedefs( output_slots ) ;
    so_class_proto_typedefs( islot_callable ) ;

    so_class_proto_typedefs( iconnector ) ;
    so_class_proto_typedefs( connectors ) ;

    namespace so_shader
    {
        so_class_proto_typedefs( ieffect ) ;
        so_class_proto_typedefs( effect ) ;
        so_class_proto_typedefs( shader ) ;
        so_class_proto_typedefs( vertex_shader ) ;
        so_class_proto_typedefs( pixel_shader ) ;
        so_struct_proto_typedefs( icustom_binding ) ;
    }

    namespace so_material
    {
        so_class_proto_typedefs( imaterial ) ;
        so_class_proto_typedefs( standard_material ) ;
    }

    namespace so_tree
    {
        so_class_proto_typedefs( node ) ;
        so_class_proto_typedefs( group ) ;
        so_class_proto_typedefs( leaf ) ;
        so_class_proto_typedefs( arg_node ) ;
        so_class_proto_typedefs( funk_node ) ;
    }

    namespace so_graph
    {        
        so_class_proto_typedefs( funk_node ) ;
        so_class_proto_typedefs( node ) ;
    }

    namespace so_visitor
    {
    }

    namespace so_kernel
    {
        so_class_proto_typedefs( ikernel ) ;
    }
}
    
#endif

