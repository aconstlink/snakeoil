#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

if( WIN81_SDK_FOUND )
    return()
endif()

set( WIN81_SDK_FOUND FALSE )

#if( NOT SNAKEOIL_TARGET_OS_WIN81 )
 #   return()
#endif()

set( WIN81_SDK_BASE_PATH "C:/Program Files (x86)/Windows Kits/8.1" )

set( WIN81_SDK_INCLUDE_DIR ${WIN81_SDK_BASE_PATH}"/Include/um" )
set( WIN81_SDK_LIBRARY_DIR ${WIN81_SDK_BASE_PATH}"/Lib/winv6.3/um/x64" )
set( WIN81_SDK_BIN_DIR ${WIN81_SDK_BASE_PATH}"/bin/x64" )

set( WIN81_SDK_FOUND TRUE )
