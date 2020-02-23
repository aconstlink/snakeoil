#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................


if( NOT FMOD_BASE_DIR )
    get_filename_component( CMAKE_CURRENT_LIST_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
    set( FMOD_BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()

####################################################################
# Find Stuff
####################################################################

#
# include dir
find_file( FMOD_HEADER "fmod.h" PATHS ${FMOD_BASE_DIR}/inc )
if( NOT FMOD_HEADER )
    message( FATAL_ERROR "[Fmod] : Can not find fmod library header" ) 
endif()
set( FMOD_INCLUDE_DIRS "${FMOD_BASE_DIR}/inc" )

#
# libraries
find_library( FMOD_LIB "fmod64_vc.lib" PATHS ${FMOD_BASE_DIR}/lib )
if( NOT FMOD_LIB )
    message( FATAL_ERROR "[Fmod] : Can not find fmod library" ) 
endif()

set( FMOD_LIBRARIES
    general ${FMOD_LIB}
   )

#
# binary dir
find_path( FMOD_BIN_DIR "fmod64.dll" PATHS ${FMOD_BASE_DIR}/lib )
if( NOT FMOD_BIN_DIR )
    message( FATAL_ERROR "[Fmod] : Can not find fmod library dll" ) 
endif()

add_library( fmod INTERFACE )
target_include_directories( fmod INTERFACE $<BUILD_INTERFACE:${FMOD_INCLUDE_DIRS}> )
target_link_libraries( fmod INTERFACE ${FMOD_LIB} )


####################################################################
# Definitions
####################################################################
#set( XLDRIVERLIB_DEFINITIONS @ALL_DEFINITIONS@ )





