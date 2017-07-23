#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( MELANGESDK_FOUND False )

#
# SECTION : Option
#
if( NOT DEFINED MELANGESDK_ENABLE_HINT )
    set( MELANGESDK_ENABLE_HINT OFF )
endif()

option( MELANGESDK_ENABLE "Enable Maxon Melange SDK" ${MELANGESDK_ENABLE_HINT} )

if( MELANGESDK_ENABLE )
    message( STATUS "[Enabled] : Maxon Melange SDK" )
else()
    message( STATUS "[Disabled] : Maxon Melange SDK" )
    return()
endif()

set( MELANGESDK_DEFINITIONS 
    -DMELANGESDK_ENABLE
    )

#
# SECTION : Config
#
set( MELANGESDK_VERSION "16" CACHE STRING "Choose the Melange SDK Version" )
set_property( CACHE MELANGESDK_VERSION PROPERTY STRINGS "16" )

set( MELANGESDK_RT_LINK "Dynamic" CACHE STRING "Choose the Melange SDK RT Linkage" )
set_property( CACHE MELANGESDK_RT_LINK PROPERTY STRINGS "Dynamic" "Static" )

if( ${MELANGESDK_RT_LINK} STREQUAL "Dynamic" )
	set( MELANGESDK_RT_LINK_STR "" )
elseif( ${MELANGESDK_RT_LINK} STREQUAL "Static" )
	set( MELANGESDK_RT_LINK_STR "_MT" )
else()
	message( FATAL_ERROR "Invalid MalageSDK linkage" )
endif()

#
# SECTION : Find
#
find_path( MELANGESDK_INCLUDE_DIRECTORY 
    NAMES "c4d_all.h" 
    HINTS  
	"${MELANGESDK_ROOTDIR}/includes"
	"$ENV{MELANGESDK_ROOTDIR}/includes"
    $ENV{MELANGESDK_INCLUDEDIR}
)

if( NOT MELANGESDK_INCLUDE_DIRECTORY )
    message( FATAL_ERROR "Maxon Melange Sdk include dir not found." )
endif() 

set( MELANGESDK_INCLUDE_DIRS 
    ${MELANGESDK_INCLUDE_DIRECTORY}
) 

find_library( MELANGESDK_LIBRARY_DEBUG
    NAMES "melangelib_debug"
    HINTS 
	"${MELANGESDK_ROOTDIR}/libraries/win/melangelib_2015${MELANGESDK_RT_LINK_STR}"
	"$ENV{MELANGESDK_ROOTDIR}/libraries/win/melangelib_2015${MELANGESDK_RT_LINK_STR}"
	$ENV{MELANGESDK_LIBRARYDIR}
    )

find_library( MELANGESDK_JPEG_LIBRARY_DEBUG
    NAMES "jpeglib_debug"
    HINTS 
	"${MELANGESDK_ROOTDIR}/libraries/win/jpeglib_2015${MELANGESDK_RT_LINK_STR}"
	"$ENV{MELANGESDK_ROOTDIR}/libraries/win/jpeglib_2015${MELANGESDK_RT_LINK_STR}"
	$ENV{MELANGESDK_JPEG_LIBRARYDIR}
    )

find_library( MELANGESDK_LIBRARY_RELEASE
    NAMES "melangelib_release" "melangelib_debug" 
    HINTS 
	"${MELANGESDK_ROOTDIR}/libraries/win/melangelib_2015${MELANGESDK_RT_LINK_STR}"
	"$ENV{MELANGESDK_ROOTDIR}/libraries/win/melangelib_2015${MELANGESDK_RT_LINK_STR}"
	$ENV{MELANGESDK_LIBRARYDIR}
    )

find_library( MELANGESDK_JPEG_LIBRARY_RELEASE
    NAMES "jpeglib_release" "jpeglib_debug"
    HINTS 
	"${MELANGESDK_ROOTDIR}/libraries/win/jpeglib_2015${MELANGESDK_RT_LINK_STR}"
	"$ENV{MELANGESDK_ROOTDIR}/libraries/win/jpeglib_2015${MELANGESDK_RT_LINK_STR}"
	$ENV{MELANGESDK_JPEG_LIBRARYDIR}
    )

set( MELANGESDK_LIBRARIES 
    debug ${MELANGESDK_LIBRARY_DEBUG}
    optimized ${MELANGESDK_LIBRARY_RELEASE}
    debug ${MELANGESDK_JPEG_LIBRARY_DEBUG}
    optimized ${MELANGESDK_JPEG_LIBRARY_RELEASE}
    general ws2_32.lib
    general winmm.lib
)

message( STATUS "[Found] : Maxon Melange SDK" )

set( MELANGESDK_FOUND True )
