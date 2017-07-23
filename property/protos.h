//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROPERTY_PROTOS_H_
#define _SNAKEOIL_PROPERTY_PROTOS_H_

#include <snakeoil/core/macros/typedef.h>

namespace so_property
{
    so_class_proto_typedefs( iproperty ) ;
    so_class_proto_typedefs( group_property) ;
    so_class_proto_typedefs( decorator_property ) ;
    so_class_proto_typedefs( single_choice_property ) ;
    so_class_proto_typedefs( multi_choice_property ) ;
    so_class_proto_typedefs( bool_property ) ;
    so_class_proto_typedefs( string_property ) ;

    so_class_proto_typedefs( ivisitor ) ;
    so_class_proto_typedefs( property_sheet ) ;
}
    
#endif

