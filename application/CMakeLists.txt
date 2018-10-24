#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( WIN32_SOURCES 
    application/win32/win32_application.h
    application/win32/win32_application.cpp
    window/win32/win32_window_handle.h
    window/win32/win32_window_handle.cpp
    window/win32/win32_window.h
    window/win32/win32_window.cpp
    window/win32/win32_info.h
    )

set( XLIB_SOURCES 
    application/xlib/xlib_application.h
    application/xlib/xlib_application.cpp
    window/xlib/xlib_window_handle.h
    window/xlib/xlib_window_handle.cpp
    window/xlib/xlib_window.h
    window/xlib/xlib_window.cpp
    window/xlib/xlib_info.h
    )

set( WGL_SOURCES
    window/wgl/wgl_context.h
    window/wgl/wgl_context.cpp
    window/wgl/wgl_window.h
    window/wgl/wgl_window.cpp
    window/wgl/wgl_info.h
)

set( GLX_SOURCES
    window/glx/glx_window.h
    window/glx/glx_window.cpp
    window/glx/glx_context.h
    window/glx/glx_context.cpp
)

set( SOURCES 
    api.h
    protos.h
    result.h
    typedefs.h

    system.h
    system.cpp

    application/iapplication.h
    application/application.h
    application/application_info.h
    application/window_application.h
    application/window_application.cpp
        
    window/irender_context.h
    window/gl_info.h
    window/gapi_info.h
    window/gl_window.h
    window/iwindow_handle.h
    window/window.h
    window/iwindow.h
    window/window_info.h
    window/window_message.h
    window/decorator_window.h
    window/decorator_window.cpp
    window/render_context_info.h
    window/iwindow_message_listener.h
    window/window_message_receiver.h
    window/window_message_receiver.cpp
    window/null/null_window.h
    window/null/null_window.cpp
    window/null/null_window_handle.h
    window/null/null_window_handle.cpp
    )

if( SNAKEOIL_WINDOW_SYSTEM_WIN32 )
    if( SNAKEOIL_TARGET_GRAPHICS_OPENGL )
        set( SOURCES ${SOURCES} ${WGL_SOURCES} )
    endif()

    
    set( SOURCES ${SOURCES} ${WIN32_SOURCES} )
    
    add_definitions( -DWIN32_LEAN_AND_MEAN )
    
elseif( SNAKEOIL_WINDOW_SYSTEM_XLIB )
    if( SNAKEOIL_TARGET_GRAPHICS_OPENGL )
        set( SOURCES ${SOURCES} ${GLX_SOURCES} )
    endif()
    set( SOURCES ${SOURCES} ${XLIB_SOURCES} )
else()
    message( FATAL_ERROR "Window System: No Os specified." )
endif()

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )
target_compile_options( ${CUR_LIB_NAME} PUBLIC ${SNAKEOIL_WINDOW_SYSTEM_DEFINES} )

target_link_libraries( ${CUR_LIB_NAME}
    PUBLIC snakeoil::gpx
    PUBLIC snakeoil::gpu
    PUBLIC snakeoil::thread
    PUBLIC snakeoil::log
    PUBLIC snakeoil::memory
    PUBLIC snakeoil::device
    PUBLIC snakeoil::io
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
