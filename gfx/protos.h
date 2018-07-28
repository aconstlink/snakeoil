//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include <snakeoil/core/macros/typedef.h>

namespace so_gfx
{
    so_class_proto_typedefs( icamera ) ;
    so_class_proto_typedefs( ilens ) ;


    so_class_proto_typedefs( text_render_2d ) ;
    so_class_proto_typedefs( text_render_2d_plug_factory ) ;

    so_class_proto_typedefs( line_render_2d ) ;
    so_class_proto_typedefs( line_render_2d_plug_factory ) ;

    so_class_proto_typedefs( rect_render_2d ) ;
    so_class_proto_typedefs( rect_render_2d_plug_factory ) ;

    so_class_proto_typedefs( circle_render_2d ) ;
    so_class_proto_typedefs( circle_render_2d_plug_factory ) ;

    so_class_proto_typedefs( image_render_2d ) ;
    so_class_proto_typedefs( image_render_2d_plug_factory ) ;

    so_class_proto_typedefs( render_2d ) ;
    so_class_proto_typedefs( sprite_manager ) ;


    so_class_proto_typedefs( simple_post ) ;
    so_class_proto_typedefs( simple_post_plug_factory ) ;

    so_class_proto_typedefs( predef_post ) ;
    so_class_proto_typedefs( predef_post_plug_factory ) ;

    so_class_proto_typedefs( predef_framebuffer ) ;
    so_class_proto_typedefs( predef_framebuffer_plug_factory ) ;

    so_class_proto_typedefs( resource_bridge ) ;
    so_class_proto_typedefs( resource_bridge_plug_factory ) ;
}
