#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# Used to find and introduce fmod to the
# application and libraries.
#

set( SEARCH_FMOD_INCLUDE_PATHS 
	"usr/local/include"
	"usr/include"
)

find_path( FMOD_INCLUDE_DIRECTORY "fmod" PATHS ${SEARCH_FMOD_INCLUDE_PATHS} )
if( FMOD_INCLUDE_DIRECTORY STREQUAL "FMOD_INCLUDE_DIRECTORY-NOTFOUND" )
	message( FATAL_ERROR "fmod include directory required" ) 
endif()

get_filename_component(FMOD_INCLUDE_DIRECTORY ${FMOD_INCLUDE_DIRECTORY} ABSOLUTE)
message( STATUS "fmod: " ${FMOD_INCLUDE_DIRECTORY})
include_directories( ${FMOD_INCLUDE_DIRECTORY} )

if( NOT SNAKEOIL_EXTERNAL_LINKAGE_REQUIRED )
return()
endif()

set( FMOD_LIBRARIES_SEARCH_PATHS 
	"C:/Windows/System32" 
	"C:/Windows" 
	"C:/Windows/System" 
	"C:/Windows/SysWOW64"
	"/usr/lib" 
	"/usr/lib64"
	"C:/Program Files (x86)/AMD APP/lib/x86_64/"
	)
	
find_library( FMOD_LIBRARY_DEBUG "fmod" PATHS ${FMOD_LIBRARIES_SEARCH_PATHS} )

find_library( FMOD_LIBRARY_RELEASE "fmod" PATHS ${FMOD_LIBRARIES_SEARCH_PATHS} )
if( NOT FMOD_LIBRARY_DEBUG OR NOT FMOD_LIBRARY_RELEASE )
	message( FATAL_ERROR "fmod Library required" )
endif()
	
	


