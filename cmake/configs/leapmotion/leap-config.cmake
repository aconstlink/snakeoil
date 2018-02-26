#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# configures the leapmotion sdk
#

if( NOT LEAP_BASE_DIR )
    get_filename_component( CMAKE_CURRENT_LIST_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
    set( LEAP_BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()

####################################################################
# Find Stuff
####################################################################

#
# include dir
find_file( LEAP_HEADER "Leap.h" PATHS ${LEAP_BASE_DIR}/include )
if( NOT LEAP_HEADER )
    message( FATAL_ERROR "[Leap] : Can not find leapmotion library header" ) 
endif()
set( LEAP_INCLUDE_DIRS "${LEAP_BASE_DIR}/include" )

#
# libraries
find_library( LEAP_LIB "Leap.lib" PATHS ${LEAP_BASE_DIR}/lib/x64 )
if( NOT LEAP_LIB )
    message( FATAL_ERROR "[Leap] : Can not find leapmotion library" ) 
endif()

set( LEAP_LIBRARIES
    general ${LEAP_LIB}
   )

#
# binary dir
find_path( LEAP_BINDIR "Leap.dll" PATHS ${LEAP_BASE_DIR}/lib/x64 )
if( NOT LEAP_BINDIR )
    message( FATAL_ERROR "[Leap] : Can not find leapmotion library dll" ) 
endif()


####################################################################
# Definitions
####################################################################






