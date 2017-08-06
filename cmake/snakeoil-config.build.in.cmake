#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# snakeoil libraries
# Automatically generated via CMake's configure_file for the build tree.
#

####################################################################
# Import targets
####################################################################
include( "@CMAKE_BINARY_DIR@/snakeoil-targets.cmake" )

if( NOT snakeoil_FIND_COMPONENTS )
    set( SNAKEOIL_LIBRARIES
    snakeoil_application
    snakeoil_appx
    snakeoil_audio
    snakeoil_audiox
    #snakeoil_scene
    #snakeoil_manager
    snakeoil_font
    snakeoil_gfx
    snakeoil_gpx
    snakeoil_gpu
    snakeoil_gli
    snakeoil_shade
    snakeoil_imex
    snakeoil_device
    #snakeoil_utility
    snakeoil_network
    snakeoil_noise
    #snakeoil_animation
    #snakeoil_collide
    snakeoil_geometry
    snakeoil_flow
    #snakeoil_resource
    snakeoil_io
    snakeoil_thread
    #snakeoil_std
    #snakeoil_physics
    #snakeoil_math
    #snakeoil_spline
    snakeoil_property
    snakeoil_memory
    snakeoil_log
    #snakeoil_core
    snakeoil_project_cars
   )
else()
    foreach( COMP ${snakeoil_FIND_COMPONENTS} )
        #message( ${COMP} )
        set( SNAKEOIL_LIBRARIES
            ${SNAKEOIL_LIBRARIES} snakeoil_${COMP}
        )
    endforeach()
endif()

####################################################################
# Definitions
####################################################################
set( SNAKEOIL_DEFINITIONS @ALL_DEFINITIONS@ )

set( SNAKEOIL_C_FLAGS "@CMAKE_C_FLAGS@" )
set( SNAKEOIL_CXX_FLAGS "@CMAKE_CXX_FLAGS@" )
####################################################################
# Directories
####################################################################
set( SNAKEOIL_INCLUDE_BASE "@CMAKE_SOURCE_DIR@" )

set( SNAKEOIL_CONFIGS_DIR 
    "${SNAKEOIL_INCLUDE_BASE}/cmake/config" )

set( SNAKEOIL_MODULES_DIR 
    "${SNAKEOIL_INCLUDE_BASE}/cmake/modules"
    "${SNAKEOIL_INCLUDE_BASE}/cmake/macros"    )

set( SNAKEOIL_INCLUDE_DIRS
    "${SNAKEOIL_INCLUDE_BASE}/.."
    "${SNAKEOIL_INCLUDE_BASE}/externals/khronos/include" )

# not perfect but currently required for dlls under windows
set( SNAKEOIL_BINDIR "@CMAKE_RUNTIME_OUTPUT_DIRECTORY@" )

#
# LUA 
#
set( LUA_ENABLE @LUA_ENABLE@ )
if( LUA_ENABLE )
    set( SNAKEOIL_LIBRARIES ${SNAKEOIL_LIBRARIES} lua )
endif()

if( LUA_ENABLE )
    set( SNAKEOIL_INCLUDE_DIRS ${SNAKEOIL_INCLUDE_DIRS} 
    "${SNAKEOIL_INCLUDE_BASE}/externals/lua-@LUA_VERSION@/include" )
endif()

####################################################################
# Find dependencies
####################################################################
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} 
    "${SNAKEOIL_MODULES_DIR}" )
    
# melange sdk
set( MELANGESDK_ENABLE_HINT @MELANGESDK_ENABLE@ )
find_package( MelangeSdk )

# OpenCtm
set( OPENCTM_ENABLE @OPENCTM_ENABLE@ )
find_package( OpenCtm )

# ProtoBuf
set( PROTOBUF_ENABLE @PROTOBUF_ENABLE@ )
find_package( Protobuf )


####################################################################
# Build-In externals paths
####################################################################
set( STB_INCLUDE_DIRS @STB_INCLUDE_DIRS@ )
set( RAPIDJSON_INCLUDE_DIRS @RAPIDJSON_INCLUDE_DIRS@ )
