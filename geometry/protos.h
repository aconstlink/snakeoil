//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_PROTOS_H_
#define _SNAKEOIL_GEOMETRY_PROTOS_H_

#include <snakeoil/core/macros/typedef.h>

namespace so_geo
{
    so_class_proto_typedefs( imesh ) ;
    so_struct_proto_typedefs( tri_mesh ) ;
    so_class_proto_typedefs( polygon_mesh ) ;
    so_struct_proto_typedefs( flat_tri_mesh ) ;

    so_class_proto_typedefs( iwire ) ;
    so_class_proto_typedefs( flat_line_mesh ) ;
}

#endif

