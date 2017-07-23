#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( RAPIDJSON_FOUND FALSE )

#
#
#

if( NOT RAPIDJSON_PATH_HINT )
    set( RAPIDJSON_PATH_HINT "${CMAKE_CURRENT_LIST_DIR}/../../externals/rapidjson-1.1.0/include" )
endif()

set( SEARCH_RAPIDJSON_INCLUDE_PATHS 
    ${RAPIDJSON_PATH_HINT}
    "usr/local/include"
    "usr/include"
    $ENV{RAPIDJSOM_INCLUDEDIR}
)

find_path( RAPIDJSON_INCLUDE_DIRECTORY 
    "rapidjson" 
    HINTS ${SEARCH_RAPIDJSON_INCLUDE_PATHS} 
    )
    
if( NOT RAPIDJSON_INCLUDE_DIRECTORY )
    message( FATAL_ERROR "RapidJson include directory required" ) 
endif()

message( STATUS "RapidJson: " ${RAPIDJSON_INCLUDE_DIRECTORY})

set( RAPIDJSON_INCLUDE_DIRS 
    ${RAPIDJSON_INCLUDE_DIRECTORY} 
    )

set( RAPIDJSON_FOUND TRUE )

