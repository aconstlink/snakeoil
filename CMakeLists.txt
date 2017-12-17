#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

cmake_minimum_required( VERSION 3.6 )
project(snakeoil CXX)

set( SNAKEOIL_MAJOR_VERSION 0 )
set( SNAKEOIL_MINOR_VERSION 1 )
set( SNAKEOIL_PATCH_VERSION 1 )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} 
    "${CMAKE_CURRENT_LIST_DIR}/cmake"
    "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/"
    )

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    
################################################################
# SECTION: Configuring
################################################################

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} 
    "${CMAKE_CURRENT_LIST_DIR}/cmake/config"
    "${CMAKE_CURRENT_LIST_DIR}/cmake/macros/"
    "${CMAKE_CURRENT_LIST_DIR}/cmake/functions/"
    )
    
include( config_operating_system )
include( config_compiler )
include( config_compiler_flags )
include( config_architecture )
include( config_instruction_set )
include( config_graphics )

include( config_build_environment )
include( config_build_library_type )
include( config_window_system )
include( config_build_configuration )

include( config_real_type )
include( config_rt_link ) 
include( config_output_directory )

include( so_visual_studio_src_dir )
include( install_headers )

################################################################
# SECTION : Build in and compilable externals
################################################################

#include( config_externals_output_directory )

# lua

add_subdirectory( externals/lua-5.3.3 )
set( LUA_VERSION 5.3.3 )
set( LUA_ENABLE ON )
set_target_properties( lua PROPERTIES FOLDER "externals" )

# ogg

set( OGG_BUILD_SHARED_LIBS ${SNAKEOIL_LIBRARY_BUILD_SHARED} )
add_subdirectory( externals/ogg-1.3.1 )
set_target_properties( ogg PROPERTIES FOLDER "externals/xiph" )


# vorbis

set( VORBIS_BUILD_SHARED_LIBS ${SNAKEOIL_LIBRARY_BUILD_SHARED} )
add_subdirectory( externals/vorbis-1.3.5 )
set_target_properties( vorbis PROPERTIES FOLDER "externals/xiph" )
set_target_properties( vorbisenc PROPERTIES FOLDER "externals/xiph" )
set_target_properties( vorbisfile PROPERTIES FOLDER "externals/xiph" )


#############################################

################################################################
# SECTION: find externals
################################################################

#
# build-in header only
find_package( Stb REQUIRED )
find_package( RapidXml REQUIRED )
#find_package( RapidJson REQUIRED )

#
# optionals
find_package( OpenCtm )
find_package( Fmod ) 

if( SNAKEOIL_TARGET_OS_WIN )
    find_package( XInput REQUIRED )
    find_package( XAudio2 REQUIRED )
    find_package( Wddk REQUIRED )
elseif( SNAKEOIL_OS_LINUX )
endif()

if( SNAKEOIL_WINDOW_SYSTEM_XLIB )
    #find_package( XLib )
endif()

# lets give every library the base path
include_directories( "${CMAKE_CURRENT_LIST_DIR}/.." )

################################################################
# SECTION: add sub directories
################################################################

if( SNAKEOIL_TARGET_OS_WIN )
set( SUBDIRS
    "audio"
    "audiox"
    "core"
    "log"
    "memory"
    "math" 
    "physics"
    "std"
    "thread"
    "io"
    "resource"
    #"variable"
    "flow"
    "geometry"
    "collide"
    "animation"
    "noise"
    "network"
    "device"
    "imex"
    "shade"
    #"gli" # depends on chosen graphics api
    "gpu"
    "gpx"
    "gfx"
    "font"
    "script"
    #"manager"
    "scene"
    "appx"
    "application"
    "spline"
    "property"
    "ui"
    )
elseif( SNAKEOIL_TARGET_OS_LIN )
set( SUBDIRS
    "core"
    "log"
    "memory"
    "math" 
    "std"
    "thread"
    "io"
    "network"
    "gpu"
    "gpx"
    )
endif()

if( SNAKEOIL_TARGET_GRAPHICS_OPENGL )
    set( SUBDIRS ${SUBDIRS} gli )
endif()

foreach( _SUBDIR ${SUBDIRS} )
    set( CUR_LIB_NAME_SHORT "${_SUBDIR}" )
    set( CUR_LIB_NAME "${PROJECT_NAME}_${_SUBDIR}" )
    set( CUR_DIR_NAME "${_SUBDIR}" )
    set( CUR_LIB_NAME_RAW "${_SUBDIR}" )
    add_subdirectory(${_SUBDIR})
endforeach()

################################################################
# SECTION : Extensions
################################################################

# project cars

add_subdirectory( extensions )

################################################################
# SECTION: in build tree fing_package generation
################################################################

# Definitions from directory
get_directory_property( SDIR_DEFS DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS )
set( DEFS )
foreach( _D ${SDIR_DEFS} )
    if( _D )
        set( DEFS ${DEFS} -D${_D} )
    endif()
endforeach()
set( ALL_DEFINITIONS ${DEFS})
#message( ${ALL_DEFINITIONS})


foreach( _SUBDIR ${SUBDIRS} )

    set( CUR_LIB_NAME "${PROJECT_NAME}_${_SUBDIR}" )
    set( TO_BE_EXPORTED ${TO_BE_EXPORTED} ${CUR_LIB_NAME} )
    
endforeach()

# do externals
set( TO_BE_EXPORTED ${TO_BE_EXPORTED} lua )

# do extensions
set( TO_BE_EXPORTED ${TO_BE_EXPORTED} "${PROJECT_NAME}_project_cars" )

configure_file( 
    "${CMAKE_SOURCE_DIR}/cmake/snakeoil-config.build.in.cmake"
    "${CMAKE_BINARY_DIR}/snakeoil-config.cmake"
    @ONLY )
    
export( TARGETS ${TO_BE_EXPORTED} FILE snakeoil-targets.cmake )


