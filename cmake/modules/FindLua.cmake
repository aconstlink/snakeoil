#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# Used to find and introduce lua to the
# application and libraries.
#

set( SEARCH_LUA_INCLUDE_PATHS 
	"usr/local/include"
	"usr/include"
	$ENV{LUA_INCLUDEDIR}
)

find_path( LUA_INCLUDE_DIRECTORY "lua.h" PATHS ${SEARCH_LUA_INCLUDE_PATHS} )
if( LUA_INCLUDE_DIRECTORY STREQUAL "LUA_INCLUDE_DIRECTORY-NOTFOUND" )
	message( FATAL_ERROR "lua include directory required" ) 
endif()

get_filename_component(LUA_INCLUDE_DIRECTORY ${LUA_INCLUDE_DIRECTORY} ABSOLUTE)
message( STATUS "lua: " ${LUA_INCLUDE_DIRECTORY})
include_directories( ${LUA_INCLUDE_DIRECTORY} )

if( NOT SNAKEOIL_EXTERNAL_LINKAGE_REQUIRED )
return()
endif()

set( LUA_LIBRARIES_SEARCH_PATHS 
	"C:/Windows/System32" 
	"C:/Windows" 
	"C:/Windows/System" 
	"C:/Windows/SysWOW64"
	"/usr/lib" 
	"/usr/lib64"
	$ENV{LUA_LIBRARYDIR}
	)
	
find_library( LUA_LIBRARY_DEBUG "lua" PATHS ${LUA_LIBRARIES_SEARCH_PATHS} )

find_library( LUA_LIBRARY_RELEASE "lua" PATHS ${LUA_LIBRARIES_SEARCH_PATHS} )
if( NOT LUA_LIBRARY_DEBUG OR NOT LUA_LIBRARY_RELEASE )
	message( FATAL_ERROR "lua Library required" )
endif()
	
	


