#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

cmake_minimum_required( VERSION 3.0 )
project( ogg C )

if( MSVC )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4456" )
endif()

set( OGG_MAJOR_VERSION 1 )
set( OGG_MINOR_VERSION 3 )
set( OGG_PATCH_VERSION 1 )
set( OGG_VERSION "${OGG_MAJOR_VERSION}.${OGG_MINOR_VERSION}.${OGG_PATCH_VERSION}" )

set( CMAKE_DEBUG_POSTFIX d )

# Build options
#option( OGG_BUILD_SHARED_LIBS "Build shared library" OFF )

if( NOT OGG_BUILD_SHARED_LIBS )
	set( OGG_BUILD_SHARED_LIBS off )
endif()

if( OGG_BUILD_SHARED_LIBS )
	set( LIBRARY_BUILD_TYPE SHARED )
else()
	set( LIBRARY_BUILD_TYPE STATIC )
endif()

set( OGG_HEADERS
    "include/ogg/ogg.h"
    "include/ogg/os_types.h"
)

set( OGG_SOURCES
    "src/bitwise.c"
    "src/framing.c"
)

if( MSVC )
    list( APPEND OGG_SOURCES win32/ogg.def )
endif()

include_directories( include )
add_library( ogg ${LIBRARY_BUILD_TYPE} ${OGG_HEADERS} ${OGG_SOURCES} )

target_include_directories( ogg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )

###########################################################
# SECTION: Install
###########################################################

set( FOLDER_NAME "${PROJECT_NAME}-${OGG_MAJOR_VERSION}.${OGG_MINOR_VERSION}.${OGG_PATCH_VERSION}")
set( BASE_INSTALL_DIR "${OGG_INSTALL_HEADER_BASE}/${FOLDER_NAME}" )

install_headers( "${OGG_HEADERS}" "${BASE_INSTALL_DIR}" )
install_library( ogg )
