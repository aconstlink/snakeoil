#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

if( NOT VECTORXL_BASE_DIR )
    get_filename_component( CMAKE_CURRENT_LIST_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
    set( VECTORXL_BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()

####################################################################
# Find Stuff
####################################################################

#
# include dir
find_file( VECTORXL_HEADER "vxlapi.h" PATHS ${VECTORXL_BASE_DIR}/bin )
if( NOT VECTORXL_HEADER )
    message( FATAL_ERROR "[VectorXL] : Can not find vxl library header" ) 
endif()
set( VECTORXL_INCLUDE_DIRS "${VECTORXL_BASE_DIR}/bin" )

#
# libraries
find_library( VECTORXL_LIB "vxlapi64.lib" PATHS ${VECTORXL_BASE_DIR}/bin )
if( NOT VECTORXL_LIB )
    message( FATAL_ERROR "[VectorXL] : Can not find vxl library" ) 
endif()

set( VECTORXL_LIBRARIES
    general ${VECTORXL_LIB}
   )

#
# binary dir
find_path( VECTORXL_BIN_DIR "vxlapi64.dll" PATHS ${VECTORXL_BASE_DIR}/bin )
if( NOT VECTORXL_BIN_DIR )
    message( FATAL_ERROR "[VectorXL] : Can not find vxl library dll" ) 
endif()


####################################################################
# Definitions
####################################################################
#set( XLDRIVERLIB_DEFINITIONS @ALL_DEFINITIONS@ )





