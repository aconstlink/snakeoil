#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( FBXSDK_FOUND False )

#
# SECTION : Option
#
option( FBXSDK_ENABLE "Enable Autodesk FBX SDK" OFF )

if( FBXSDK_ENABLE )
	message( STATUS "[Enabled] : Autodesk FBX SDK" )
else()
	message( STATUS "[Disabled] : Autodesk FBX SDK" )
	return()
endif()

set( FBXSDK_DEFINITIONS 
	-DFBXSDK_ENABLE
	)
	
#
# SECTION : Config
#
set( FBXSDK_VERSION "2016.1" CACHE STRING "Choose the Fbx Sdk Version" )
set_property( CACHE FBXSDK_VERSION PROPERTY STRINGS "2016.1" )

#
# SECTION : Find
#
find_path( FBXSDK_INCLUDE_DIRECTORY 
	NAMES "fbxsdk.h" 
	HINTS  
	$ENV{FBXSDK_INCLUDEDIR} 
	"C:/Program Files/Autodesk/FBX/FBX SDK/2015.1/include"
)

if( NOT FBXSDK_INCLUDE_DIRECTORY )
	message( FATAL_ERROR "FBX Sdk include dir not found." )
endif() 

set( FBXSDK_INCLUDE_DIRS 
	${FBXSDK_INCLUDE_DIRECTORY}
) 

find_library( FBXSDK_LIBRARY_DEBUG
	NAMES "fbxsdk-md" "libfbxsdk-md" 
	HINTS 
		$ENV{FBXSDK_LIBRARYDIR}/debug
	)
	
find_library( FBXSDK_LIBRARY_RELEASE
	NAMES "fbxsdk-md" "libfbxsdk-md" 
	HINTS 
		$ENV{FBXSDK_LIBRARYDIR}/release
	)
	
set( FBXSDK_LIBRARIES 
	debug ${FBXSDK_LIBRARY_DEBUG}
	optimized ${FBXSDK_LIBRARY_RELEASE}
)

message( STATUS "[Found] : FBX SDK" )

set( FBXSDK_FOUND True )
