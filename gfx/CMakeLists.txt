#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( GL33_SHADERS
  circle/gl33/shader/circle_render_2d.vs.glsl
  circle/gl33/shader/circle_render_2d.ps.glsl
  image/gl33/shader/image_render_2d.vs.glsl
  image/gl33/shader/image_render_2d.ps.glsl
  line/gl33/shader/line_render_2d.vs.glsl
  line/gl33/shader/line_render_2d.ps.glsl
  rect/gl33/shader/rect_render_2d.vs.glsl
  rect/gl33/shader/rect_render_2d.ps.glsl
  text/gl33/shader/text_render_2d.vs.glsl
  text/gl33/shader/text_render_2d.ps.glsl
  plugs/post_process/gl33/shader/gl33_toscreen_post.vs.glsl
  plugs/post_process/gl33/shader/gl33_toscreen_post.ps.glsl
  plugs/post_process/gl33/shader/gl33_vignette_post.vs.glsl
  plugs/post_process/gl33/shader/gl33_vignette_post.ps.glsl
)

set( GL33_SOURCES 
  text/gl33/gl33_text_render_2d_plug.h
  text/gl33/gl33_text_render_2d_plug.cpp
  line/gl33/gl33_line_render_2d_plug.h
  line/gl33/gl33_line_render_2d_plug.cpp
  image/gl33/gl33_image_render_2d_plug.h
  image/gl33/gl33_image_render_2d_plug.cpp
  rect/gl33/gl33_rect_render_2d_plug.h
  rect/gl33/gl33_rect_render_2d_plug.cpp
  circle/gl33/gl33_circle_render_2d_plug.h
  circle/gl33/gl33_circle_render_2d_plug.cpp
  plugs/framebuffer/gl33/gl33_framebuffer_begin_plug.h
  plugs/framebuffer/gl33/gl33_framebuffer_begin_plug.cpp
  plugs/framebuffer/gl33/gl33_framebuffer_end_plug.h
  plugs/framebuffer/gl33/gl33_framebuffer_end_plug.cpp
  plugs/post_process/gl33/gl33_singlepass_post_plug.h
  plugs/post_process/gl33/gl33_singlepass_post_plug.cpp
  plugs/post_process/gl33/gl33_toscreen_post_plug.h
  plugs/post_process/gl33/gl33_toscreen_post_plug.cpp
  plugs/post_process/gl33/gl33_vignette_post_plug.h
  plugs/post_process/gl33/gl33_vignette_post_plug.cpp
  plugs/framebuffer/gl33/gl33_predef_framebuffer_plug.h
  plugs/framebuffer/gl33/gl33_predef_framebuffer_plug.cpp
)
  
set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h    

    camera/icamera.h
    camera/generic_camera.h
    camera/generic_camera.cpp
    camera/ilens.h
    camera/pinhole_lens.h
    camera/pinhole_lens.cpp
    
    sprite/sprite_manager.h
    sprite/sprite_manager.cpp

    text/text_render_2d.h
    text/text_render_2d.cpp
    text/text_render_2d_plug_factory.h
    text/text_render_2d_plug_factory.cpp
    text/text_render_2d_shared_data.h

    line/line_render_2d.h
    line/line_render_2d.cpp
    line/line_render_2d_plug_factory.h
    line/line_render_2d_plug_factory.cpp
    line/line_render_2d_shared_data.h

    rect/rect_render_2d.h
    rect/rect_render_2d.cpp
    rect/rect_render_2d_plug_factory.h
    rect/rect_render_2d_plug_factory.cpp
    rect/rect_render_2d_shared_data.h

    circle/circle_render_2d.h
    circle/circle_render_2d.cpp
    circle/circle_render_2d_plug_factory.h
    circle/circle_render_2d_plug_factory.cpp
    circle/circle_render_2d_shared_data.h

    image/image_render_2d.h
    image/image_render_2d.cpp
    image/image_render_2d_plug_factory.h
    image/image_render_2d_plug_factory.cpp
    image/image_render_2d_shared_data.h

    render/render_2d.h
    render/render_2d.cpp

    plugs/framebuffer/framebuffer_begin.h
    plugs/framebuffer/framebuffer_begin.cpp
    plugs/framebuffer/framebuffer_begin_plug_factory.h
    plugs/framebuffer/framebuffer_begin_plug_factory.cpp
    plugs/framebuffer/framebuffer_end.h
    plugs/framebuffer/framebuffer_end.cpp
    plugs/framebuffer/framebuffer_end_plug_factory.h
    plugs/framebuffer/framebuffer_end_plug_factory.cpp    
    
    plugs/framebuffer/predef_framebuffer.h
    plugs/framebuffer/predef_framebuffer.cpp
    plugs/framebuffer/predef_framebuffer_plug_factory.h
    plugs/framebuffer/predef_framebuffer_plug_factory.cpp
    plugs/framebuffer/predef_framebuffer_types.h

    plugs/post_process/predef_post_type.h
    plugs/post_process/predef_post.h
    plugs/post_process/predef_post.cpp
    plugs/post_process/predef_post_plug_factory.h
    plugs/post_process/predef_post_plug_factory.cpp

    enums/align.h
    )

set( SOURCES ${SOURCES} )

if( SNAKEOIL_TARGET_GRAPHICS_OPENGL_33 )
  set( SOURCES ${SOURCES} ${GL33_SOURCES} )
  set( SOURCES ${SOURCES} ${GL33_SHADERS} )
endif()

so_vs_src_dir( SOURCES )

##########################################################
# configure config.h
configure_file( "${CMAKE_SOURCE_DIR}/gfx/gfx_config.build.in.h"
  "${CMAKE_BINARY_DIR}/gfx/gfx_config.h" @ONLY )

set( SOURCES_CONFIG ${CMAKE_BINARY_DIR}/gfx/gfx_config.h )

include_directories( "${CMAKE_BINARY_DIR}/gfx" )
##########################################################

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} ${SOURCES_CONFIG} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_GFX_EXPORT
        #-DSNAKEOIL_FONT_IMPORT
        #-DSNAKEOIL_GPX_IMPORT
        #-DSNAKEOIL_GPU_IMPORT
        #-DSNAKEOIL_THREAD_IMPORT
        #-DSNAKEOIL_LOG_IMPORT
        #-DSNAKEOIL_MEMORY_IMPORT
        )

    # snakeoil
    target_link_libraries( ${CUR_LIB_NAME}
      snakeoil_font
      snakeoil_gpx
      snakeoil_gpu
      snakeoil_log
      snakeoil_memory
      snakeoil_thread
      snakeoil_flow
      snakeoil_gpu
    )
endif()

###########################################################
# SECTION: Install
###########################################################

install_shaders( "${GL33_SHADERS}" "bin/${CUR_LIB_NAME_SHORT}" )
install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install( FILES ${CMAKE_BINARY_DIR}/gfx/config.h DESTINATION "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
