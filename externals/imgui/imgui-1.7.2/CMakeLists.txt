
#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

cmake_minimum_required( VERSION 3.0 )
project( imgui CXX )

# we need the math library
include_directories( "${CMAKE_CURRENT_LIST_DIR}/../../../.." )

if( MSVC )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244 /wd4324 /wd4310" )
endif()

set( IMGUI_MAJOR_VERSION 1 )
set( IMGUI_MINOR_VERSION 6 )
set( IMGUI_PATCH_VERSION 7 )
set( IMGUI_VERSION "${IMGUI_MAJOR_VERSION}.${IMGUI_MINOR_VERSION}.${IMGUI_PATCH_VERSION}" )

set( CMAKE_DEBUG_POSTFIX d )

set( INCLUDES 
  "include/imgui/imgui.h"
  "include/imgui/imconfig.h"
  "include/imgui/imgui_internal.h"
  "include/imgui/imstb_rectpack.h"
  "include/imgui/imstb_textedit.h"
  "include/imgui/imstb_truetype.h"
)

set( SOURCES
  ${INCLUDES}
    "src/imgui.cpp"
    "src/imgui_demo.cpp"
    "src/imgui_draw.cpp"
    "src/imgui_widgets.cpp"
)

#add_definitions( -DIMGUI_DEFINE_MATH_OPERATORS )
#include_directories( "./include/imgui" "./include/imgui/internal" ) 

add_library( imgui STATIC ${SOURCES} )
set_target_properties( imgui PROPERTIES LINKER_LANGUAGE CXX )
target_include_directories( imgui 
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include 
  PRIVATE "./include/imgui" 
  PRIVATE "./include/imgui/internal" ) 

target_compile_options(imgui PUBLIC -DIMGUI_DEFINE_MATH_OPERATORS )

###########################################################
# SECTION: Install
###########################################################

set( FOLDER_NAME "${PROJECT_NAME}-${IMGUI_MAJOR_VERSION}.${IMGUI_MINOR_VERSION}.${IMGUI_PATCH_VERSION}")
set( BASE_INSTALL_DIR "${IMGUI_INSTALL_HEADER_BASE}/${FOLDER_NAME}" )

install_headers( "${SOURCES}" "${BASE_INSTALL_DIR}" )
install_library( imgui )

