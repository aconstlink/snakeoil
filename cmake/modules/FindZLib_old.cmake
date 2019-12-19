#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# 
#

set( SEARCH_ZLIB_INCLUDE_PATHS
	"usr/local/include"
	"usr/include"
	$ENV{ZLIB_INCLUDEDIR}
)

find_path( ZLIB_INCLUDE_DIRECTORY 
	NAMES "zlib" 
	HINTS ${SEARCH_ZLIB_INCLUDE_PATHS} 
	)
	
if( NOT ZLIB_INCLUDE_DIRECTORY )
	message( FATAL_ERROR "zlib include directory not found." ) 
endif()

get_filename_component( ZLIB_INCLUDE_DIRECTORY ${ZLIB_INCLUDE_DIRECTORY} ABSOLUTE )
message( STATUS "zlib: " ${ZLIB_INCLUDE_DIRECTORY} )

include_directories( ${ZLIB_INCLUDE_DIRECTORY} )

if( NOT SNAKEOIL_EXTERNAL_LINKAGE_REQUIRED ) 
return()
endif()

set( ZLIB_LIBRARIES_SEARCH_PATHS 
	"/usr/lib" 
	"/usr/lib64"
	$ENV{ZLIB_LIBRARYDIR}
	)
	
find_library( ZLIB_LIBRARY_DEBUG 
	NAMES "zlibd" 
	HINTS ${ZLIB_LIBRARIES_SEARCH_PATHS} )

find_library( ZLIB_LIBRARY_RELEASE 
	NAMES "zlib" 
	HINTS ${ZLIB_LIBRARIES_SEARCH_PATHS} 
	)

if( NOT LIBZ_LIBRARY_DEBUG OR NOT LIBZ_LIBRARY_RELEASE )
	message( FATAL_ERROR "libz Library required" )
endif()

