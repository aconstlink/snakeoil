#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# configures the Mosquitto MQTT api
# very simple cmake file for configuring the api
# http://mosquitto.org/
#

if( NOT MOSQUITTO_BASE_DIR )
    get_filename_component( CMAKE_CURRENT_LIST_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
    set( MOSQUITTO_BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()

####################################################################
# Find Stuff
####################################################################

#
# include dir
find_file( MOSQUITTO_HEADER "mosquitto.h" PATHS ${MOSQUITTO_BASE_DIR}/include )
if( NOT MOSQUITTO_HEADER )
    message( FATAL_ERROR "[Mosquitto] : Can not find Mosquitto library header" ) 
endif()
set( MOSQUITTO_INCLUDE_DIRS "${MOSQUITTO_BASE_DIR}/include" )

#
# libraries
find_library( MOSQUITTO_LIB "mosquitto.lib" PATHS ${MOSQUITTO_BASE_DIR} )
if( NOT MOSQUITTO_LIB )
    message( FATAL_ERROR "[Mosquitto] : Can not find mosquitto library" ) 
endif()

find_library( MOSQUITTOPP_LIB "mosquittopp.lib" PATHS ${MOSQUITTO_BASE_DIR} )
if( NOT MOSQUITTOPP_LIB )
    message( FATAL_ERROR "[Mosquitto] : Can not find mosquittopp library" ) 
endif()

set( MOSQUITTO_LIBRARIES
    general ${MOSQUITTO_LIB}
    general ${MOSQUITTOPP_LIB}
   )

#
# binary dir
find_path( MOSQUITTO_BINDIR "mosquitto.dll" PATHS ${MOSQUITTO_BASE_DIR} )
if( NOT MOSQUITTO_BINDIR )
    message( FATAL_ERROR "[Mosquitto] : Can not find Mosquitto library dll" ) 
endif()


####################################################################
# Definitions
####################################################################






