cmake_minimum_required( VERSION 3.0 )
project( libusb C )




if( NOT LUA_INSTALL_HEADER_BASE )
  set( INSTALL_HEADER_BASE "" )
endif()

if( MSVC )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} " )
endif()

set( USB_MAJOR_VERSION 1 )
set( USB_MINOR_VERSION 0 )
set( USB_PATCH_VERSION 22 )
set( USB_VERSION "${LUA_MAJOR_VERSION}.${LUA_MINOR_VERSION}.${LUA_PATCH_VERSION}" )

set( SOURCE_FILES
  libusb/core.c
  libusb/descriptor.c
libusb/hotplug.c
libusb/io.c
libusb/strerror.c
libusb/sync.c
)

set( HEADER_FILES
libusb/hotplug.h
libusb/libusb.h
libusb/version.h
)

set( SOURCES 
    ${SOURCE_FILES} 
    ${HEADER_FILES}
)

if( WIN32 )

  set( SOURCES ${SOURCES}
    libusb/os/threads_windows.c
    libusb/os/threads_windows.h
    libusb/os/windows_common.h
    libusb/os/windows_nt_common.h
    libusb/os/windows_nt_common.c
    libusb/os/windows_nt_shared_types.h
    libusb/os/windows_usbdk.c
    libusb/os/windows_winusb.c
    libusb/os/windows_winusb.h
    libusb/os/poll_windows.h
    libusb/os/poll_windows.c
    libusb/libusb-1.0.def
    libusb/libusbd-1.0.def
    )
  
  add_definitions( -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE )

else()
  message( ERROR "LibUsb Cmake not implemented" )
endif()

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/libusb/config.in.h"
  "${CMAKE_BINARY_DIR}/externals/libusb/generated/config.h" @ONLY )

set( SOURCES ${SOURCES} "${CMAKE_BINARY_DIR}/externals/libusb/generated" )
include_directories("${CMAKE_BINARY_DIR}/externals/libusb/generated")

set( USB_BUILD_TYPE SHARED )
include_directories( "./libusb" )
add_library( libusb ${USB_BUILD_TYPE} ${SOURCES} )
set_target_properties( libusb PROPERTIES LINKER_LANGUAGE C )


