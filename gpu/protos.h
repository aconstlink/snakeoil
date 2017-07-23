//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_PROTOS_H_
#define _SNAKEOIL_GPU_PROTOS_H_

#include <snakeoil/core/macros/typedef.h>

namespace so_gpu
{
    so_class_proto_typedefs( iapi ) ;
    so_class_proto_typedefs( api_object ) ;
    so_class_proto_typedefs( api_object_helper ) ;
    so_class_proto_typedefs( idata_context ) ;

    namespace so_gles
    {
        so_class_proto_typedefs( igles_api ) ;
        so_class_proto_typedefs( igles_20_api ) ;
        so_class_proto_typedefs( igles_32_api ) ;

        so_class_proto_typedefs( igles_20_plug ) ;
        so_class_proto_typedefs( igles_32_plug ) ;
    }

    namespace so_gl
    {
        so_class_proto_typedefs( igl_api ) ;
        so_class_proto_typedefs( igl_20_api ) ;
        so_class_proto_typedefs( igl_33_api ) ;
        so_class_proto_typedefs( igl_45_api ) ;

        so_class_proto_typedefs( igl_plug ) ;
        so_class_proto_typedefs( igl_20_plug ) ;
    }

    namespace so_d3d
    {
        so_class_proto_typedefs( id3d_10_api ) ;
        so_class_proto_typedefs( id3d_11_api ) ;
        so_class_proto_typedefs( id3d_12_api ) ;
    }

    namespace so_vulkan
    {}





    namespace so_driver
    {
        so_class_proto_typedefs(driver) ;

        so_class_proto_typedefs(driver_api) ;
        so_struct_proto_typedefs(object) ;
        so_struct_proto_typedefs(context) ;
        
        namespace so_null
        {
            so_class_proto_typedefs(null_driver) ;
            so_struct_proto_typedefs(null_context) ;
        }
        
        namespace so_gl
        {
            so_class_proto_typedefs(gl_driver) ;
            so_struct_proto_typedefs(gl_context) ;
        }        
    }
    typedef so_driver::driver_ptr_t driver_ptr_t ;
    
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

    so_class_proto_typedefs( gpu_manager ) ;
}

#endif

