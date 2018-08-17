#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

include_directories( ${IMGUI_INCLUDE_DIRS} )

set( GL33_SOURCES 
  plugs/gl33/gl33_imgui_plug.h
  plugs/gl33/gl33_imgui_plug.cpp
)
  
set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h

    system/system.h
    system/system.cpp

    plugs/imgui.h
    plugs/imgui.cpp
    plugs/imgui_plug_factory.h
    plugs/imgui_plug_factory.cpp
    )

set( SOURCES ${SOURCES} )

if( SNAKEOIL_TARGET_GRAPHICS_OPENGL_33 )
  set( SOURCES ${SOURCES} ${GL33_SOURCES} )
endif()

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_IMGUI_EXPORT
        -DSNAKEOIL_DEVICE_IMPORT
        -DSNAKEOIL_GPX_IMPORT
        -DSNAKEOIL_GPU_IMPORT
        -DSNAKEOIL_THREAD_IMPORT
        -DSNAKEOIL_LOG_IMPORT
        -DSNAKEOIL_MEMORY_IMPORT
        )

    # snakeoil
    target_link_libraries( ${CUR_LIB_NAME}
      snakeoil_device
      snakeoil_gpx
      snakeoil_gpu
      snakeoil_log
      snakeoil_memory
      snakeoil_thread
      imgui
    )
endif()

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
