#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#n
#
#

if( NOT MICROCHIP_3DTPAD_BASE_DIR )
    get_filename_component( CMAKE_CURRENT_LIST_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
    set( MICROCHIP_3DTPAD_BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()

####################################################################
# Find Stuff
####################################################################

#
# include dir
find_file( MICROCHIP_3DTPAD_HEADER "hmi_api.h" PATHS ${MICROCHIP_3DTPAD_BASE_DIR}/api/include )
if( NOT MICROCHIP_3DTPAD_HEADER )
    message( FATAL_ERROR "[Microchip 3D Pad] : Can not find Microchip 3d Touch Pad library header" ) 
endif()
set( MICROCHIP_3DTPAD_INCLUDE_DIRS "${MICROCHIP_3DTPAD_BASE_DIR}/api/include" )

set( MICROCHIP_3DTPAD_LINKAGE "Static" CACHE STRING "Dynamic/static linkage")
set_property( CACHE MICROCHIP_3DTPAD_LINKAGE PROPERTY STRINGS "Dynamic" "Static" )

#
# libraries

if( MICROCHIP_3DTPAD_LINKAGE STREQUAL "Static" )
  
  set( _LIBPATH "api/lib-static/" )

elseif( MICROCHIP_3DTPAD_LINKAGE STREQUAL "Dynamic" )

  set( MICROCHIP_3DTPAD_DEFINITIONS HMI_API_DYNAMIC )
  set( _LIBPATH "api/lib/" )

endif()


find_library( MICROCHIP_3DTPAD_LIB "mchp_hmi.lib" PATHS ${MICROCHIP_3DTPAD_BASE_DIR}/${_LIBPATH} )
if( NOT MICROCHIP_3DTPAD_LIB )
  message( FATAL_ERROR "[Microchip 3D Pad] : Can not find Microchip mchp_hmi.lib library" ) 
endif()

unset( _LIBPATH )

set( MICROCHIP_3DTPAD_LIBRARIES
    general ${MICROCHIP_3DTPAD_LIB}
   )

#
# binary dir
find_path( MICROCHIP_3DTPAD_BINDIR "mchp_hmi.dll" PATHS ${MICROCHIP_3DTPAD_BASE_DIR}/api/lib )
if( NOT MICROCHIP_3DTPAD_BINDIR )
    message( FATAL_ERROR "[Microchip 3D Pad] : Can not find mchp_hmi.dll library" ) 
endif()


####################################################################
# Definitions
####################################################################






