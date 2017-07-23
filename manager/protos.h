//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_PROTOS_H_
#define _SNAKEOIL_MANAGER_PROTOS_H_

#include <snakeoil/core/macros/typedef.h>

namespace so_manager
{
    so_class_proto_typedefs( code_manager ) ;
    so_class_proto_typedefs( camera_manager ) ;

    namespace so_graphics
    {
        so_class_proto_typedefs( shader_manager ) ;
        so_class_proto_typedefs( program_manager ) ;
        so_class_proto_typedefs( geometry_manager ) ;
        so_class_proto_typedefs( variable_manager ) ;
        so_class_proto_typedefs( framebuffer_manager ) ;
        so_class_proto_typedefs( image_manager ) ;
        so_class_proto_typedefs( texture_manager ) ;
    }

    so_class_proto_typedefs( manager_registry ) ;
    so_class_proto_typedefs( system ) ;
}

#endif
