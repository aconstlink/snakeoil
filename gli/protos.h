//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GLI_PROTOS_H_
#define _SNAKEOIL_GLI_PROTOS_H_

#include <snakeoil/core/macros/typedef.h>

namespace so_gli
{
    namespace so_driver
    {
        so_class_proto_typedefs(driver) ;
        so_class_proto_typedefs(driver_async) ;

        so_class_proto_typedefs(driver_api) ;
        so_struct_proto_typedefs(object) ;
        so_struct_proto_typedefs(context) ;
        
        namespace so_null
        {
            so_class_proto_typedefs(null_driver) ;
            so_struct_proto_typedefs(null_context) ;
        }
        
        namespace so_gli
        {
            so_class_proto_typedefs(gl_driver) ;
            so_struct_proto_typedefs(gl_context) ;
        }        
    }
    typedef so_driver::driver_ptr_t driver_ptr_t ;
    typedef so_driver::driver_async_ptr_t driver_async_ptr_t ;
    
    so_class_proto_typedefs(object) ;
    
    so_class_proto_typedefs(viewport) ;

    so_class_proto_typedefs(shader) ;
    so_class_proto_typedefs(vertex_shader) ;
    so_class_proto_typedefs(pixel_shader) ;
    so_class_proto_typedefs(geometry_shader) ;

    so_class_proto_typedefs(program) ;
    so_class_proto_typedefs(config) ;
    
    so_class_proto_typedefs(shader_variable) ;
    so_class_proto_typedefs(variable_set) ;
    so_class_proto_typedefs(variable) ;
    so_class_proto_typedefs(itexture_variable) ;
    so_class_proto_typedefs(idata_buffer_variable) ;

    so_class_proto_typedefs(buffer) ;
    so_class_proto_typedefs(ivertex_buffer) ;
    so_class_proto_typedefs(iindex_buffer) ;
    so_class_proto_typedefs(idata_buffer) ;
    so_class_proto_typedefs(feedback_buffer) ;

    so_class_proto_typedefs(framebuffer) ;
    so_class_proto_typedefs(framebuffer_2d) ;
    so_class_proto_typedefs(framebuffer_3d) ;

    so_class_proto_typedefs(image) ;
    so_class_proto_typedefs(iimage_2d) ;
    so_class_proto_typedefs(iimage_3d) ;

    so_class_proto_typedefs(texture) ;
    so_class_proto_typedefs(texture_2d) ;
    so_class_proto_typedefs(texture_3d) ;
    so_class_proto_typedefs(depth_texture_2d) ;

    so_class_proto_typedefs(sync) ;
}

#endif

