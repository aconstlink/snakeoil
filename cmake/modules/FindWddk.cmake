#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#if( NOT SNAKEOIL_TARGET_OS_WIN7 ) 
#  return() 
#endif()

find_path( WDDK_INCLUDE_DIRECTORY "hidsdi.h" PATHS "C:/Program Files (x86)/Windows Kits/8.1/Include/shared" )

#find_library( WDDK_LIBRARY hid.lib PATHS "C:/WinDDK/7600.16385.1/lib/win7/amd64" )
set( WDDK_LIBRARIES hid.lib ) 

if( NOT WDDK_INCLUDE_DIRECTORY )
  message( "Can not find wddk include directory" )
endif()

#if( NOT WDDK_LIBRARY )
#  message( "Can not find wddk library" )
#endif()


set( WDDK_INCLUDE_DIRS ${WDDK_INCLUDE_DIRECTORY} )
