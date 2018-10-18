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
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )

target_link_libraries( ${CUR_LIB_NAME}
      PUBLIC snakeoil::device
      PUBLIC snakeoil::gpx
      PUBLIC snakeoil::gpu
      PUBLIC snakeoil::log
      PUBLIC snakeoil::memory
      PUBLIC snakeoil::thread
      PUBLIC imgui
    )

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
