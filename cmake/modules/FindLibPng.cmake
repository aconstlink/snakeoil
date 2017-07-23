#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# Used to find and introduce LibPng to the
# application and libraries.
#

set( SEARCH_LIBPNG_INCLUDE_PATHS 
	"usr/local/include"
	"usr/include"
)

find_path( LIBPNG_INCLUDE_DIRECTORY "png" "libpng" PATHS ${SEARCH_LIBPNG_INCLUDE_PATHS} )
if( LIBPNG_INCLUDE_DIRECTORY STREQUAL "LIBPNG_INCLUDE_DIRECTORY-NOTFOUND" )
	message( FATAL_ERROR "LibPNG include directory required" ) 
endif()

get_filename_component(LIBPNG_INCLUDE_DIRECTORY ${LIBPNG_INCLUDE_DIRECTORY} ABSOLUTE)
message( STATUS "libpng: " ${LIBPNG_INCLUDE_DIRECTORY})
include_directories( ${LIBPNG_INCLUDE_DIRECTORY} )

if( NOT SNAKEOIL_EXTERNAL_LINKAGE_REQUIRED )
return()
endif()

set( LIBPNG_LIBRARIES_SEARCH_PATHS 
	"C:/Windows/System32" 
	"C:/Windows" 
	"C:/Windows/System" 
	"C:/Windows/SysWOW64"
	"/usr/lib" 
	"/usr/lib64"
	"C:/Program Files (x86)/AMD APP/lib/x86_64/"
	)
	
find_library( LIBPNG_LIBRARY_DEBUG "png" PATHS ${LIBPNG_LIBRARIES_SEARCH_PATHS} )
find_library( LIBPNG_LIBRARY_RELEASE "png" PATHS ${LIBPNG_LIBRARIES_SEARCH_PATHS} )
if( NOT LIBPNG_LIBRARY_DEBUG OR NOT LIBPNG_LIBRARY_RELEASE )
	message( FATAL_ERROR "LibPNG Library required" )
endif()
	
	


