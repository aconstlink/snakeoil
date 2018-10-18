#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

if( SNAKEOIL_TARGET_GRAPHICS_OPENGL )
    include_directories(
        ${OPENGL_INCLUDE_DIRS}
    )
endif()

set( SOURCES 
    api.h
    protos.h
    result.h
    typedefs.h
    
    application/appx_application.h
    application/appx_application.cpp
    
    system/states.h
    system/appx_system.h
    system/appx_system.cpp
    system/init_data.h
    system/update_data.h
    system/render_data.h
    system/iappx_context.h
    system/appx_context.h
    system/appx_context.cpp
    system/iwindow_state_informer.h
    system/window_state_informer.h
    system/window_state_informer.cpp
    system/event_data.h
    
    app/iapp.h
    app/app.h
    app/app.cpp
    
    )
    
so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )

target_link_libraries( ${CUR_LIB_NAME}
  PUBLIC snakeoil::application
  PUBLIC snakeoil::imex
  PUBLIC snakeoil::audiox
  PUBLIC snakeoil::audio
  PUBLIC snakeoil::gpx
  PUBLIC snakeoil::gpu
  PUBLIC snakeoil::thread
  PUBLIC snakeoil::log
  PUBLIC snakeoil::memory
  PUBLIC snakeoil::device
  PUBLIC snakeoil::io  )

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
