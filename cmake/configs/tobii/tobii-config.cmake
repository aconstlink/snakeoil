#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# configures the tobii eye tracking stream sdk
#

if( NOT TOBII_BASE_DIR )
    get_filename_component( CMAKE_CURRENT_LIST_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
    set( TOBII_BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()

####################################################################
# Find Stuff
####################################################################

#
# include dir
find_file( TOBII_HEADER "tobii.h" PATHS ${TOBII_BASE_DIR}/include/tobii )
if( NOT TOBII_HEADER )
    message( FATAL_ERROR "[Tobii] : Can not find tobii stream library header" ) 
endif()
set( TOBII_INCLUDE_DIRS "${TOBII_BASE_DIR}/include" )

#
# libraries
find_library( TOBII_LIB "tobii_stream_engine.lib" PATHS ${TOBII_BASE_DIR}/lib/x64 )
if( NOT TOBII_LIB )
    message( FATAL_ERROR "[Tobii] : Can not find tobii stream  library" ) 
endif()

set( TOBII_LIBRARIES
    general ${TOBII_LIB}
   )

#
# binary dir
find_path( TOBII_BINDIR "tobii_stream_engine.dll" PATHS ${TOBII_BASE_DIR}/lib/x64 )
if( NOT TOBII_BINDIR )
    message( FATAL_ERROR "[Tobii] : Can not find tobii stream library dll" ) 
endif()


####################################################################
# Definitions
####################################################################






