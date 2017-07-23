#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# Used to find and introduce qtproperty to the
# application and libraries.
#

set( SEARCH_QTPROPERTY_INCLUDE_PATHS 
	"usr/local/include"
	"usr/include"
)

find_path( QTPROPERTY_INCLUDE_DIRECTORY "propertybrowser" PATHS ${SEARCH_QTPROPERTY_INCLUDE_PATHS} )
if( QTPROPERTY_INCLUDE_DIRECTORY STREQUAL "QTPROPERTY_INCLUDE_DIRECTORY-NOTFOUND" )
	message( FATAL_ERROR "propertybrowser include directory required" ) 
endif()

get_filename_component(QTPROPERTY_INCLUDE_DIRECTORY ${QTPROPERTY_INCLUDE_DIRECTORY} ABSOLUTE)
message( STATUS "propertybrowser: " ${QTPROPERTY_INCLUDE_DIRECTORY})
include_directories( ${QTPROPERTY_INCLUDE_DIRECTORY} )

# must be linked into the static library
add_definitions( -DQT_QTPROPERTYBROWSER_IMPORT) 

if( NOT SNAKEOIL_EXTERNAL_LINKAGE_REQUIRED ) 
return()
endif()

set( QTPROPERTY_LIBRARIES_SEARCH_PATHS 
	"C:/Windows/System32" 
	"C:/Windows" 
	"C:/Windows/System" 
	"C:/Windows/SysWOW64"
	"/usr/lib" 
	"/usr/lib64"
	"C:/Program Files (x86)/AMD APP/lib/x86_64/"
	)
	
find_library( QTPROPERTY_LIBRARY_DEBUG "propertybrowser" PATHS ${QTPROPERTY_LIBRARIES_SEARCH_PATHS} )

find_library( QTPROPERTY_LIBRARY_RELEASE "propertybrowser" PATHS ${QTPROPERTY_LIBRARIES_SEARCH_PATHS} )
if( NOT QTPROPERTY_LIBRARY_DEBUG OR NOT QTPROPERTY_LIBRARY_RELEASE )
	message( FATAL_ERROR "qtproperty Library required" )
endif()
	
	


