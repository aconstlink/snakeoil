#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# Used to find and introduce lua to the
# application and libraries.
#

set( SEARCH_LUAJIT_INCLUDE_PATHS 
	"usr/local/include"
	"usr/include"
	$ENV{LUAJIT_INCLUDEDIR}
)

find_path( 	
	LUAJIT_INCLUDE_DIRECTORY "lua.h" "luajit.h" 
	HINTS ${SEARCH_LUAJIT_INCLUDE_PATHS} 
	)
			
if( LUAJIT_INCLUDE_DIRECTORY STREQUAL "LUAJIT_INCLUDE_DIRECTORY-NOTFOUND" )
	message( FATAL_ERROR "lua include directory required" ) 
endif()

get_filename_component( LUAJIT_INCLUDE_DIRECTORY ${LUAJIT_INCLUDE_DIRECTORY} ABSOLUTE )
message( STATUS "LuaJit: " ${LUAJIT_INCLUDE_DIRECTORY} )
include_directories( ${LUAJIT_INCLUDE_DIRECTORY} )

if( NOT SNAKEOIL_EXTERNAL_LINKAGE_REQUIRED )
return()
endif()

set( LUAJIT_LIBRARIES_SEARCH_PATHS 
	"C:/Windows/System32" 
	"C:/Windows" 
	"C:/Windows/System" 
	"C:/Windows/SysWOW64"
	"/usr/lib" 
	"/usr/lib64"
	$ENV{LUAJIT_LIBRARYDIR}
	)
	
find_library( 
	LUAJIT_LIBRARY_GENERAL "lua51"
	HINTS ${LUAJIT_LIBRARIES_SEARCH_PATHS} 
	)

if( NOT LUAJIT_LIBRARY_GENERAL )
	message( FATAL_ERROR "LuaJit library required" )
endif()
	
	


