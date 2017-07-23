#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# This module looks for the gtest components.
#

find_library( GTEST_LIBRARY gtest DOC "Specify gtest library" 
    HINTS /usr/local/lib /usr/local/lib64 )
	
find_path( GTEST_INCLUDE_DIRECTORY "gtest" DOC "Specify gtest inlcude path"
            HINTS /usr/local/include )
			
if( ${GTEST_INCLUDE_DIRECTORY} STREQUAL GTEST_INCLUDE_DIRECTORY-NOTFOUND )
	message( FATAL_ERROR "why: Please specify google test components." )
endif()

set( GTEST_FOUND TRUE )

include_directories(${GTEST_INCLUDE_DIRECTORY})


