//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_DAE_ELEMENTS_DAE_PROTOS_H_
#define _SNAKEOIL_IMEX_MODULE_DAE_ELEMENTS_DAE_PROTOS_H_

#include "../../../typedefs.h"

namespace so_imex
{
    namespace so_dae
    {
        so_class_proto_typedefs( visitor ) ;
        so_class_proto_typedefs( node ) ;

        so_struct_proto_typedefs( collada ) ;
        so_struct_proto_typedefs( asset ) ;
        so_struct_proto_typedefs( unit ) ;
        so_struct_proto_typedefs( geometry ) ;
        so_struct_proto_typedefs( mesh ) ;

        so_struct_proto_typedefs( source ) ;
        so_struct_proto_typedefs( float_array ) ;
        so_struct_proto_typedefs( technique_common ) ;
        so_struct_proto_typedefs( accessor ) ;
        so_struct_proto_typedefs( param ) ;
        so_struct_proto_typedefs( vertices ) ;
        so_struct_proto_typedefs( input ) ;
        so_struct_proto_typedefs( polygons ) ;
        so_struct_proto_typedefs( primitives ) ;
        so_struct_proto_typedefs( data ) ;

        so_struct_proto_typedefs( library_effects ) ;
        so_struct_proto_typedefs( library_materials ) ;
        so_struct_proto_typedefs( library_geometries ) ;
    }
}

#endif