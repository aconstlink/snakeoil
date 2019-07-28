#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

cmake_minimum_required( VERSION 3.6 )
project(snakeoil CXX)

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

# lua

set( LUA_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
add_subdirectory( externals/lua-5.3.3 )
set( LUA_VERSION 5.3.3 )
set( LUA_ENABLE ON )
set_target_properties( lua PROPERTIES FOLDER "snakeoil/externals" )

# khronos
set( KHRONOS_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
add_subdirectory( externals/khronos )

# rapidxml
# also see find_package
set( RAPIDXML_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
add_subdirectory( externals/rapidxml-1.13 )

# rapidjson
# todo

# stb
# also see find_package
set( STB_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals/stb-2.31" )
add_subdirectory( externals/stb-2.31 )

# ogg
set( OGG_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
set( OGG_BUILD_SHARED_LIBS ${SNAKEOIL_LIBRARY_BUILD_SHARED} )
add_subdirectory( externals/ogg-1.3.1 )
set_target_properties( ogg PROPERTIES FOLDER "snakeoil/externals/xiph" )

# vorbis
set( VORBIS_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
set( VORBIS_BUILD_SHARED_LIBS ${SNAKEOIL_LIBRARY_BUILD_SHARED} )

add_subdirectory( externals/vorbis-1.3.5 )
set_target_properties( vorbis PROPERTIES FOLDER "snakeoil/externals/xiph" )
set_target_properties( vorbisenc PROPERTIES FOLDER "snakeoil/externals/xiph" )
set_target_properties( vorbisfile PROPERTIES FOLDER "snakeoil/externals/xiph" )

# imgui
set( IMGUI_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
add_subdirectory( externals/imgui-1.6.8 )
set( IMGUI_VERSION 1.6.8 )
set_target_properties( imgui PROPERTIES FOLDER "snakeoil/externals" )
set( IMGUI_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/externals/imgui-1.6.8/include" )

# rtmidi
set( RTMIDI_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
add_subdirectory( externals/rtmidi-3.0.0 )
set( RTMIDI_VERSION 3.0.0 )
set_target_properties( rtmidi PROPERTIES FOLDER "snakeoil/externals/rtmidi" )
set_target_properties( rtmidi_static PROPERTIES FOLDER "snakeoil/externals/rtmidi" )
set_target_properties( uninstall PROPERTIES FOLDER "snakeoil/externals/rtmidi" )
set( RTMIDI_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/externals/rtmidi-3.0.0" )

target_compile_definitions( rtmidi PRIVATE RTMIDI_DO_NOT_ENSURE_UNIQUE_PORTNAMES )
target_compile_definitions( rtmidi_static PRIVATE RTMIDI_DO_NOT_ENSURE_UNIQUE_PORTNAMES )

if( SNAKEOIL_COMPILER_MSC )
  target_compile_options( rtmidi PRIVATE /wd4267 /wd4996 )
  target_compile_options( rtmidi_static PRIVATE /wd4267 /wd4996 )
endif()

# libusb
#set( LIBUSB_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
#add_subdirectory( externals/libusb-1.0.22 )
#set( LIBUSB_VERSION 1.0.22 )
#set( LIBUSB_ENABLE ON )
#set_target_properties( libusb PROPERTIES FOLDER "snakeoil/externals" )
#set( LIBUSB_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/externals/libusb-1.0.22/libusb" )

#zlib
# todo

#############################################

################################################################
# SECTION: find externals
################################################################

#
# build-in header only
find_package( Stb REQUIRED )
find_package( RapidXml REQUIRED )
find_package( RapidJson REQUIRED )

#
# optionals
#find_package( OpenCtm )
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
    #"scene"
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
export(TARGETS complete NAMESPACE snakeoil:: APPEND FILE ${PROJECT_NAME}-targets.cmake)

################################################################
# SECTION : Extensions
################################################################

# project cars

#add_subdirectory( extensions )

################################################################
# SECTION : Export externals
################################################################

# do externals
# TODO this should be done in the libs respective target files and 
# included in snakeoils config file
list( APPEND TO_BE_EXPORTED lua imgui )
export( TARGETS ${TO_BE_EXPORTED} NAMESPACE snakeoil:: APPEND FILE snakeoil-targets.cmake )

################################################################
# SECTION: in build tree fing_package generation
################################################################
configure_file( 
    "${SNAKEOIL_SOURCE_DIR}/cmake/snakeoil-config.build.in.cmake"
    "${SNAKEOIL_BINARY_DIR}/snakeoil-config.cmake"
    @ONLY )
