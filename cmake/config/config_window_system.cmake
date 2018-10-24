#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SNAKEOIL_WINDOW_SYSTEM_CONFIGURED FALSE )

if( NOT SNAKEOIL_OS_CONFIGURED )
    message( FATAL_ERROR "Configured Target OS required." )
endif()

set( SNAKEOIL_WINDOW_SYSTEM_NULL ON )
set( SNAKEOIL_WINDOW_SYSTEM_WIN32 OFF )
set( SNAKEOIL_WINDOW_SYSTEM_XLIB OFF )
set( SNAKEOIL_WINDOW_SYSTEM_WAYLAND OFF )

if( SNAKEOIL_TARGET_OS_WIN )
    
    set( SNAKEOIL_TARGET_WINDOW_SYSTEM_API "win32" CACHE STRING 
    "Select the window system api to be compiled into the package." )

    set_property(CACHE SNAKEOIL_TARGET_WINDOW_SYSTEM_API PROPERTY STRINGS "null" "win32" )

    if( SNAKEOIL_TARGET_WINDOW_SYSTEM_API STREQUAL "null" )
        
        set( SNAKEOIL_WINDOW_SYSTEM_NULL ON )
        set( SNAKEOIL_WINDOW_SYSTEM_DEFINES 
          -DSNAKEOIL_WINDOW_SYSTEM_NULL )
    elseif( SNAKEOIL_TARGET_WINDOW_SYSTEM_API STREQUAL "win32" )
    
        set( SNAKEOIL_WINDOW_SYSTEM_NULL OFF )
        set( SNAKEOIL_WINDOW_SYSTEM_WIN32 ON )
        set( SNAKEOIL_WINDOW_SYSTEM_DEFINES 
          -DSNAKEOIL_WINDOW_SYSTEM_WIN32 )
    else()
        message( FATAL_ERROR "Invalid window system parameter" )
    endif()
    
    
    
elseif( SNAKEOIL_TARGET_OS_LIN )

    set( SNAKEOIL_TARGET_WINDOW_SYSTEM_API "null" CACHE STRING 
    "Select the window system api to be compiled into the package." )

    set_property(CACHE SNAKEOIL_TARGET_WINDOW_SYSTEM_API PROPERTY STRINGS 
        "null" "x11" "wayland" )

    if( SNAKEOIL_TARGET_WINDOW_SYSTEM_API STREQUAL "null" )
        
        set( SNAKEOIL_WINDOW_SYSTEM_NULL ON )
        set( SNAKEOIL_WINDOW_SYSTEM_DEFINES 
          -DSNAKEOIL_WINDOW_SYSTEM_WIN32 )
        message( "Null window system driver used" )

    elseif( SNAKEOIL_TARGET_WINDOW_SYSTEM_API STREQUAL "x11" )

        set( SNAKEOIL_WINDOW_SYSTEM_XLIB ON )
        set( SNAKEOIL_WINDOW_SYSTEM_DEFINES 
          -DSNAKEOIL_WINDOW_SYSTEM_XLIB )
        message( "X11 window system driver used" )
        message( FATAL_ERROR "Not supported at the moment" )
    
    elseif( SNAKEOIL_TARGET_WINDOW_SYSTEM_API STREQUAL "wayland" )
        
        set( SNAKEOIL_WINDOW_SYSTEM_WAYLAND ON )
        set( SNAKEOIL_WINDOW_SYSTEM_DEFINES 
          -DSNAKEOIL_WINDOW_SYSTEM_WAYLAND )
        message( "Wayland window system driver used" )
        message( FATAL_ERROR "Not supported at the moment" )

    else()
        message( FATAL_ERROR "Invalid window system parameter" )
    endif()

else()
    message( FATAL_ERROR "Window System Required" )
endif()


#
# Print Info
# 
if( SNAKEOIL_WINDOW_SYSTEM_WIN32 )
    message( STATUS "[Window System] : Win32    " )
elseif( SNAKEOIL_WINDOW_SYSTEM_XLIB )
    message( STATUS "[Window System] : XLib" )
elseif( SNAKEOIL_WINDOW_SYSTEM_WAYLAND )
    message( STATUS "[Window System] : Wayland" )
else()
    message( STATUS "[Window System] : Unknown" )
endif()

set( SNAKEOIL_WINDOW_SYSTEM_CONFIGURED TRUE )


