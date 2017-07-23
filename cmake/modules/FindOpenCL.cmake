#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# Used to find and introduce OpenCL file to the
# application and libraries.
#

set( SEARCH_CL_INCLUDE_PATHS 
	"usr/local/include"
	"usr/include"
	"C:/Program Files (x86)/AMD APP/include"
)

find_path( CL_INCLUDE_DIRECTORY "CL" PATHS ${SEARCH_CL_INCLUDE_PATHS} )
if( CL_INCLUDE_DIRECTORY STREQUAL "CL_INCLUDE_DIRECTORY-NOTFOUND" )
	message( FATAL_ERROR "OpenCL include directory required" ) 
endif()

get_filename_component(CL_INCLUDE_DIRECTORY ${CL_INCLUDE_DIRECTORY} ABSOLUTE)
message( STATUS "OpenCL: " ${CL_INCLUDE_DIRECTORY})
include_directories( ${CL_INCLUDE_DIRECTORY} )

if( NOT SNAKEOIL_EXTERNAL_LINKAGE_REQUIRED )
return()
endif() 

set( CL_LIBRARIES_SEARCH_PATHS 
	"C:/Windows/System32" 
	"C:/Windows" 
	"C:/Windows/System" 
	"C:/Windows/SysWOW64"
	"/usr/lib" 
	"/usr/lib64"
	"C:/Program Files (x86)/AMD APP/lib/x86_64/"
	)
	
find_library( CL_LIBRARY "OpenCL" PATHS ${CL_LIBRARIES_SEARCH_PATHS} )
if( CL_LIBRARY STREQUAL "CL_LIBRARY-NOTFOUND")
	message( FATAL_ERROR "OpenCL Library required" )
endif()
	
	


