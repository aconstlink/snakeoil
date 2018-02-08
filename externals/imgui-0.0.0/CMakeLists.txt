
#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

cmake_minimum_required( VERSION 3.0 )
project( imgui CXX )

# we need the math library
include_directories( "${CMAKE_CURRENT_LIST_DIR}/../../.." )

if( MSVC )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244 /wd4324 /wd4310" )
endif()

set( IMGUI_MAJOR_VERSION 0 )
set( IMGUI_MINOR_VERSION 0 )
set( IMGUI_PATCH_VERSION 0 )
set( IMGUI_VERSION "${IMGUI_MAJOR_VERSION}.${IMGUI_MINOR_VERSION}.${IMGUI_PATCH_VERSION}" )

set( CMAKE_DEBUG_POSTFIX d )

set( INCLUDES 
  "include/imgui/imgui.h"
  "include/imgui/imconfig.h"
  "include/imgui/internal/imgui_internal.h"
  "include/imgui/internal/stb_rect_pack.h"
  "include/imgui/internal/stb_textedit.h"
  "include/imgui/internal/stb_truetype.h"
)

set( SOURCES
  ${INCLUDES}
    "src/imgui.cpp"
    "src/imgui_demo.cpp"
    "src/imgui_draw.cpp"
)

add_definitions( -DIMGUI_DEFINE_MATH_OPERATORS )
include_directories( "./include/imgui" "./include/imgui/internal" ) 

add_library( imgui STATIC ${SOURCES} )
set_target_properties( imgui PROPERTIES LINKER_LANGUAGE CXX )

###########################################################
# SECTION: Install
###########################################################

set( FOLDER_NAME "${PROJECT_NAME}-${IMGUI_MAJOR_VERSION}.${IMGUI_MINOR_VERSION}.${IMGUI_PATCH_VERSION}")
set( BASE_INSTALL_DIR "externals/${FOLDER_NAME}" )
install_headers( "${SOURCES}" "${BASE_INSTALL_DIR}" )


install( TARGETS imgui DESTINATION "${BASE_INSTALL_DIR}/lib" )

