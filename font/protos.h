//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FONT_PROTOS_H_
#define _SNAKEOIL_FONT_PROTOS_H_

#include "typedefs.h"

namespace so_font
{
    so_class_proto_typedefs( isystem ) ;
    
    so_class_proto_typedefs( idriver ) ;
    so_struct_proto_typedefs( idriver_item ) ;
    so_class_proto_typedefs( driver_object ) ;
    so_class_proto_typedefs( driver_api ) ;

    so_class_proto_typedefs( face ) ;
    so_class_proto_typedefs( glyph ) ;

    so_class_proto_typedefs( itechnique ) ;

    so_class_proto_typedefs( face_config ) ;

    so_class_proto_typedefs( glyph_atlas_2d ) ;

}

#endif

