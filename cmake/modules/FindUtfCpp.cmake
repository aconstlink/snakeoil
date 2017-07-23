#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( UTFCPP_FOUND False )

#
# 
#

if( NOT UTFCPP_PATH_HINT )
	set( UTFCPP_PATH_HINT "${CMAKE_CURRENT_LIST_DIR}/../../externals/utfcpp-2.3.4/include" )
endif()

set( SEARCH_UTFCPP_INCLUDE_PATHS 
	"usr/local/include"
	"usr/include"
	${UTFCPP_PATH_HINT}
	$ENV{UTFCPP_INCLUDEDIR}
)

find_path( UTFCPP_INCLUDE_DIRECTORY 
	NAMES "utfcpp"
	HINTS ${SEARCH_UTFCPP_INCLUDE_PATHS} 
	)
	
if( NOT UTFCPP_INCLUDE_DIRECTORY )
	message( FATAL_ERROR "utfcpp include directory required" ) 
endif()

set( UTFCPP_INCLUDE_DIRS 
	${UTFCPP_INCLUDE_DIRECTORY} 
	)
	
message( STATUS "utfcpp: " ${UTFCPP_INCLUDE_DIRECTORY})

set( UTFCPP_FOUND True )
