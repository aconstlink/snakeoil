#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

include_directories(
    ${OPENGL_INCLUDE_DIRS}
)

set( SOURCE_ENUMS
    enums/types.h
    enums/type_traits.h

    enums/access_types.h
    enums/attribute_state.h
    enums/blend_factors.h
    enums/blend_functions.h
    enums/clear_modes.h
    enums/color_types.h
    enums/cull_modes.h
    enums/feedback_buffer_types.h
    enums/fill_modes.h
    enums/framebuffer_attachments.h
    enums/front_faces.h
    enums/geometry_shader_input_types.h
    enums/geometry_shader_output_types.h
    enums/matrix_modes.h
    enums/memory_hints.h
    enums/primitive_types.h
    enums/render_states.h
    enums/shader_variable_types.h
    enums/texture_buffer_formats.h
    enums/texture_compare_functions.h
    enums/texture_cube_faces.h
    enums/texture_filters.h
    enums/texture_formats.h
    enums/texture_types.h
    enums/texture_wraps.h
    enums/timer_types.h
    enums/color_targets.h
    enums/vertex_attributes.h
    enums/sync_fence.h
    enums/image_formats.h
    enums/image_type.h
    enums/pixel_type.h
    enums/data_buffer_formats.h
)

set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h
    object.h
    object.cpp
    state_set.h
    
    converter/convert_color_to_texture_format.h
    converter/convert_core_gfx_type.h
    converter/convert_primitive_to_gs_type.h
    converter/convert_type_to_image_format.h
    converter/convert_type_to_pixel_type.h
    converter/convert_image_to_shader_variable_type.h

    shader/shader_types.h
    shader/shader.h
    shader/shader.cpp
    shader/vertex_shader.h
    shader/vertex_shader.cpp
    shader/geometry_shader.h
    shader/geometry_shader.cpp
    shader/pixel_shader.h
    shader/pixel_shader.cpp

    program/config.h
    program/config.cpp
    program/program.h
    program/program.cpp

    buffer/buffer.h
    buffer/buffer.cpp
    buffer/buffer_layout.h
    buffer/vertex_buffer.hpp
    buffer/index_buffer.hpp
    buffer/pixel_buffer.hpp
    buffer/idata_buffer.h
    buffer/data_buffer.hpp
    buffer/vertex_buffer_layout.h
    buffer/ivertex_buffer.h
    buffer/iindex_buffer.h
    buffer/feedback_buffer.h
    buffer/feedback_buffer.cpp
    buffer/geometry.h
    buffer/geometry.cpp

    image/image.h
    image/image.cpp
    image/color_image_2d.hpp
    image/depth_image_2d.hpp
    image/iimage_2d.h
    image/iimage_3d.h
    image/image_creator.h
    image/image_creator.cpp

    texture/texture.h
    texture/texture.cpp
    texture/texture_2d.h
    texture/texture_2d.cpp
    texture/texture_3d.h
    texture/texture_3d.cpp
    texture/depth_texture_2d.h
    texture/depth_texture_2d.cpp
    texture/render_texture_2d.h
    texture/render_texture_2d.cpp
        
    variable/shader_variable.h
    variable/shader_variable.cpp
    variable/variable_set.h
    variable/variable_set.cpp
    variable/variable.h
    variable/variable.cpp
    variable/data_variable.hpp
    variable/texture_variable.hpp
    variable/data_buffer_variable.hpp

    framebuffer/framebuffer.h
    framebuffer/framebuffer.cpp
    framebuffer/framebuffer_2d.h
    framebuffer/framebuffer_2d.cpp
    framebuffer/framebuffer_3d.h
    framebuffer/framebuffer_3d.cpp

    viewport/viewport_2d.h

    sync/sync.h
    sync/sync.cpp

	manager/gpu_manager.h
	manager/gpu_manager.cpp
	
    ${SOURCE_ENUMS}
    )


set( GL_ANY_SOURCES
    api/gl/gl_any/log.h
    api/gl/gl_any/log.cpp
    api/gl/gl_any/gl_context.h
    api/gl/gl_any/objects/vertex_shader.h
    api/gl/gl_any/objects/geometry_shader.h
    api/gl/gl_any/objects/image.h
    api/gl/gl_any/objects/pixel_shader.h
    api/gl/gl_any/objects/vertex_buffer.h
    api/gl/gl_any/objects/index_buffer.h
    api/gl/gl_any/objects/program.h
    api/gl/gl_any/objects/framebuffer.h
    api/gl/gl_any/objects/config.h
    api/gl/gl_any/objects/shader.h
    api/gl/gl_any/objects/shader_variable.h
    api/gl/gl_any/objects/sampler.h
    api/gl/gl_any/objects/data_buffer.h
    api/gl/gl_any/objects/variable_set.h
    api/gl/gl_any/objects/variable.h
    api/gl/gl_any/objects/sync.h
    api/gl/gl_any/objects/feedback_buffer.h
    
    api/gl/gl_any/converter/gl_convert_attribute_bit.h
    api/gl/gl_any/converter/gl_convert_blend_factor.h
    api/gl/gl_any/converter/gl_convert_blend_function.h
    api/gl/gl_any/converter/gl_convert_clear_mode.h
    api/gl/gl_any/converter/gl_convert_color_attachment.h
    api/gl/gl_any/converter/gl_convert_color_type.h
    api/gl/gl_any/converter/gl_convert_cull_mode.h
    api/gl/gl_any/converter/gl_convert_feedback_buffer_type.h
    api/gl/gl_any/converter/gl_convert_feedback_primitve_type.h
    api/gl/gl_any/converter/gl_convert_fill_mode.h
    api/gl/gl_any/converter/gl_convert_front_face.h
    api/gl/gl_any/converter/gl_convert_geometry_shader_type.h
    api/gl/gl_any/converter/gl_convert_memory_hint.h
    api/gl/gl_any/converter/gl_convert_primitive_type.h
    api/gl/gl_any/converter/gl_convert_texture_compare_function.h
    api/gl/gl_any/converter/gl_convert_texture_filter.h
    api/gl/gl_any/converter/gl_convert_render_state.h
    api/gl/gl_any/converter/gl_convert_data_buffer_format.h
    api/gl/gl_any/converter/gl_convert_texture_format.h
    api/gl/gl_any/converter/gl_convert_texture_wrap.h
    api/gl/gl_any/converter/gl_convert_timer_type.h
    api/gl/gl_any/converter/gl_convert_type.h
    api/gl/gl_any/converter/gl_convert_uniform_type.h
    api/gl/gl_any/converter/gl_convert_sync_fence.h
    api/gl/gl_any/converter/gl_convert_image_format.h
)

set( GL_API_SOURCES
    api/gl/igl_api.h
    )
    
set( GL_20_API_SOURCES
    api/gl/igl_20_api.h
    api/gl/gl_20/gl_20_api.h
    api/gl/gl_20/gl_20_api.cpp
    )
    
set( GL_33_API_SOURCES
    api/gl/igl_33_api.h
    api/gl/gl_33/gl_33_api.h
    api/gl/gl_33/gl_33_api.cpp
    api/gl/gl_33/gl_33_api_link.cpp
    api/gl/gl_33/gl_33_api_compile.cpp
    api/gl/gl_33/gl_33_api_creation.cpp
    api/gl/gl_33/gl_33_api_execution.cpp
    api/gl/gl_33/gl_33_api_memory.cpp
    api/gl/gl_33/gl_33_api_state.cpp
    api/gl/gl_33/gl_33_api_variable.cpp
    api/gl/gl_33/gl_33_api_framebuffer.cpp
    )
    
set( GL_45_API_SOURCES
    api/gl/igl_45_api.h
    api/gl/gl_45/gl_45_api.h
    api/gl/gl_45/gl_45_api.cpp
    )
    
set( SOURCES ${SOURCES} 
    api/api_types.h
    api/iapi.h
    api/info_structs.h
    api/api_object.h
    api/api_object_helper.h
    api/api_object_helper.cpp
    api/null/inull_api.h
    api/null/null_api.h
    api/null/null_api.cpp    
    )
    
if( SNAKEOIL_TARGET_GRAPHICS_OPENGL )
    set( SOURCES ${SOURCES}
        ${GL_API_SOURCES}
        ${GL_ANY_SOURCES}
        #${GL_20_API_SOURCES} 
        ${GL_33_API_SOURCES} 
        #${GL_45_API_SOURCES} 
    )
endif() 

so_vs_src_dir( SOURCES )


add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
#add_library( ${CUR_LIB_NAME} STATIC ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_GPU_EXPORT
        -DSNAKEOIL_IMEX_IMPORT
        -DSNAKEOIL_THREAD_IMPORT
        -DSNAKEOIL_LOG_IMPORT
        -DSNAKEOIL_MEMORY_IMPORT
        )

    # snakeoil
    target_link_libraries( ${CUR_LIB_NAME}
      snakeoil_imex
      snakeoil_log
      snakeoil_memory
      snakeoil_thread
    )
    
    if( SNAKEOIL_TARGET_GRAPHICS_OPENGL )
        add_definitions( 
            -DSNAKEOIL_GL_IMPORT
            )
        
        #snakeoil
        target_link_libraries( ${CUR_LIB_NAME}
            snakeoil_gli
        )

        #3rd party
        target_link_libraries( ${CUR_LIB_NAME}
            ${GL_LIBRARIES}
        )
    endif()    

endif()

