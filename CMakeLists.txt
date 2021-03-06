#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

cmake_minimum_required( VERSION 3.6 )
project(snakeoil CXX)

if( POLICY CMP0079 )
  cmake_policy( SET CMP0079 NEW )
endif()

set( SNAKEOIL_MAJOR_VERSION 0 )
set( SNAKEOIL_MINOR_VERSION 1 )
set( SNAKEOIL_PATCH_VERSION 8 )

################################################################
# SECTION: Configuring
################################################################

set( SNAKEOIL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( SNAKEOIL_BINARY_DIR ${PROJECT_BINARY_DIR} )
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} 
  "${CMAKE_CURRENT_LIST_DIR}/cmake"
  "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/"
  "${CMAKE_CURRENT_LIST_DIR}/cmake/config"
  "${CMAKE_CURRENT_LIST_DIR}/cmake/macros/"
  "${CMAKE_CURRENT_LIST_DIR}/cmake/functions/"
  )

set( CMAKE_INSTALL_PREFIX "${SNAKEOIL_BINARY_DIR}/install/snakeoil-${SNAKEOIL_MAJOR_VERSION}.${SNAKEOIL_MINOR_VERSION}.${SNAKEOIL_PATCH_VERSION}" CACHE STRING "Install prefix" FORCE )    

include( config_export_target )
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
include( install_shaders )
include( install_library )

so_export_clear()

################################################################
# SECTION : Build in and compilable externals
################################################################


add_subdirectory( "externals/lua" )
add_subdirectory( "externals/khronos" )
add_subdirectory( "externals/rtmidi" )
add_subdirectory( "externals/rapidxml" )
add_subdirectory( "externals/stb" )
add_subdirectory( "externals/ogg" )
add_subdirectory( "externals/vorbis" )
add_subdirectory( "externals/imgui" )

#add_subdirectory( "externals/libusb" )
add_subdirectory( "externals/glm" )
add_subdirectory( "externals/assimp" )

#zlib
# todo

#############################################

################################################################
# SECTION: find externals
################################################################

# optionals
#find_package( OpenCtm )
find_package( Fmod ) 

if( SNAKEOIL_TARGET_OS_WIN )
    find_package( XInput REQUIRED )
    find_package( XAudio2 REQUIRED )
 #   find_package( Wddk REQUIRED )
elseif( SNAKEOIL_OS_LINUX )
endif()

if( SNAKEOIL_WINDOW_SYSTEM_XLIB )
    #find_package( XLib )
endif()

################################################################
# SECTION: add sub directories
################################################################

set( SUBDIRS_HEADER_ONLY 
  core
  math
  physics
  collide
  animation
  spline
)
set( SUBDIRS
    "audio"
    "audiox"
    "log"
    "memory"
    "std"
    "thread"
    "io"
    "dsp"
    "event"
    #"resource"
    #"variable"
    "flow"
    "geometry"
    "noise"
    "network"
    "device"
    "imex"
    "degui"
    "shade"
    #"gli" # depends on chosen graphics api
    "gpu"
    "gpx"
    "gfx"
    "font"
    #"script"
    #"manager"
    "scene"
    "appx"
    "application"
    "property"
    #"ui"
    )


if( SNAKEOIL_TARGET_GRAPHICS_OPENGL )
    list( APPEND SUBDIRS gli )
endif()

#
# header-only
#
foreach( _SUBDIR ${SUBDIRS_HEADER_ONLY} )
    
    set( CUR_LIB_NAME "${_SUBDIR}" )
    set( CUR_ALIAS_NAME "${PROJECT_NAME}::${_SUBDIR}" )
    add_subdirectory(${_SUBDIR})

    target_compile_options( ${CUR_LIB_NAME} PRIVATE ${SNAKEOIL_COMPILER_OPTIONS_PRIVATE})
    target_include_directories( ${CUR_LIB_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.." )

    set_target_properties( ${CUR_LIB_NAME} PROPERTIES FOLDER "snakeoil" )

    set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )
    set_target_properties( ${CUR_LIB_NAME} PROPERTIES CXX_STANDARD ${SNAKEOIL_CXX_STANDARD} )
endforeach()

#
# full libs
#
add_library( complete INTERFACE )
add_library( ${PROJECT_NAME}::complete ALIAS complete )

foreach( _SUBDIR ${SUBDIRS} )
    set( CUR_LIB_NAME_SHORT "${_SUBDIR}" )
    set( CUR_LIB_NAME "${_SUBDIR}" )
    string(TOUPPER ${CUR_LIB_NAME} CUR_LIB_NAME_UPPER)
    set( CUR_DIR_NAME "${_SUBDIR}" )
    set( CUR_LIB_NAME_RAW "${_SUBDIR}" )
    set( CUR_ALIAS_NAME "${PROJECT_NAME}::${_SUBDIR}" )
    set( CUR_FILE_NAME "${PROJECT_NAME}_${_SUBDIR}" )
    add_subdirectory(${_SUBDIR})

    set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )
    set_target_properties( ${CUR_LIB_NAME} PROPERTIES OUTPUT_NAME ${CUR_FILE_NAME})
    set_target_properties( ${CUR_LIB_NAME} PROPERTIES CXX_STANDARD ${SNAKEOIL_CXX_STANDARD} )
    if( SNAKEOIL_LIBRARY_BUILD_SHARED )
      target_compile_options( ${CUR_LIB_NAME} 
        PRIVATE -DSNAKEOIL_${CUR_LIB_NAME_UPPER}_EXPORT 
        INTERFACE -DSNAKEOIL_${CUR_LIB_NAME_UPPER}_IMPORT )    
    endif()
    target_compile_options( ${CUR_LIB_NAME} PRIVATE ${SNAKEOIL_COMPILER_OPTIONS_PRIVATE})
    target_include_directories( ${CUR_LIB_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.." )

    set_target_properties( ${CUR_LIB_NAME} PROPERTIES FOLDER "snakeoil" )
    
    target_link_libraries( complete INTERFACE ${CUR_LIB_NAME} )
endforeach()
add_subdirectory(sox)
export(TARGETS complete NAMESPACE snakeoil:: APPEND FILE ${PROJECT_NAME}-targets.cmake)

################################################################
# SECTION : Extensions
################################################################

# project cars

#add_subdirectory( extensions )

################################################################
# SECTION : Export externals
################################################################

################################################################
# SECTION: in build tree fing_package generation
################################################################
configure_file( 
    "${SNAKEOIL_SOURCE_DIR}/cmake/snakeoil-config.build.in.cmake"
    "${SNAKEOIL_BINARY_DIR}/snakeoil-config.cmake"
    @ONLY )
