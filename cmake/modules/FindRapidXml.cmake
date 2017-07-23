#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( RAPIDXML_FOUND FALSE )

#
#
#

if( NOT RAPIDXML_PATH_HINT )
    set( RAPIDXML_PATH_HINT "${CMAKE_CURRENT_LIST_DIR}/../../externals/rapidxml-1.13/include" )
endif()

set( SEARCH_RAPIDXML_INCLUDE_PATHS 
    ${RAPIDXML_PATH_HINT}
    "usr/local/include"
    "usr/include"
    $ENV{RAPIDXML_INCLUDEDIR}
)

find_path( RAPIDXML_INCLUDE_DIRECTORY 
    "rapidxml" 
    HINTS ${SEARCH_RAPIDXML_INCLUDE_PATHS} 
    )

if( NOT RAPIDXML_INCLUDE_DIRECTORY )
    message( FATAL_ERROR "RapidXML include directory required" ) 
endif()

message( STATUS "RapidXML: " ${RAPIDXML_INCLUDE_DIRECTORY})

set( RAPIDXML_INCLUDE_DIRS 
    ${RAPIDXML_INCLUDE_DIRECTORY} 
    )

#include_directories( ${RAPID_XML_INCLUDE_DIRECTORY} )


set( RAPIDXML_FOUND TRUE )

