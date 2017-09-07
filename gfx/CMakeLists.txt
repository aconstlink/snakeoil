#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( GL33_SOURCES 
  text/gl33/gl33_text_render_2d_plug.h
  text/gl33/gl33_text_render_2d_plug.cpp
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
    
    text/text_render_2d.h
    text/text_render_2d.cpp
    text/text_render_2d_plug_factory.h
    text/text_render_2d_plug_factory.cpp
    text/text_render_2d_shared_data.h

    enums/align.h
    )

set( SOURCES ${SOURCES} )

set( SOURCES ${SOURCES} ${GL33_SOURCES} )

so_vs_src_dir( SOURCES )

add_definitions( -DGFX_SOURCE_DIR="${CMAKE_CURRENT_LIST_DIR}" ) 

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_GFX_EXPORT
        -DSNAKEOIL_FONT_IMPORT
        -DSNAKEOIL_GPX_IMPORT
        -DSNAKEOIL_GPU_IMPORT
        -DSNAKEOIL_THREAD_IMPORT
        -DSNAKEOIL_LOG_IMPORT
        -DSNAKEOIL_MEMORY_IMPORT
        )

    # snakeoil
    target_link_libraries( ${CUR_LIB_NAME}
      snakeoil_font
      snakeoil_gpx
      snakeoil_gpu
      snakeoil_log
      snakeoil_memory
      snakeoil_thread
      snakeoil_gpu
    )
endif()
