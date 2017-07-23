#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

if( DXSDK_FOUND )
    return()
endif()

set( DXSDK_FOUND FALSE )

#
# 1. Use env in order to find dx sdk location
#
set( DXSDK_BASE_PATH $ENV{DXSDK_DIR} )

if( NOT DXSDK_BASE_PATH )
    message( FATAL_ERROR "Please Install the latest Microsoft DirectX SDK" )
endif()

#
# 2. Set paths for include, library and bin
#
set( DXSDK_INCLUDE_DIR_HINT "${DXSDK_BASE_PATH}/Include" )
find_path( DXSDK_INCLUDE_DIR "d3d11.h" HINTS ${DXSDK_INCLUDE_DIR_HINT} )
if( NOT DXSDK_INCLUDE_DIR )
    message( FATAL_ERROR "DirectX Sdk include directory not found" )
endif()
set( DXSDK_LIBRARY_DIR "${DXSDK_BASE_PATH}/Lib/x64" )
set( DXSDK_BIN_DIR "${DXSDK_BASE_PATH}/Developer Runtime/x64" )

unset( DXSDK_INCLUDE_DIR_HINT )

set( DXSDK_FOUND TRUE )

