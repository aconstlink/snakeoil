cmake_minimum_required(VERSION 3.0 )
project( vorbis C )

if( MSVC )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4456 /wd4244 /wd4324 /wd4310 /wd4305 /wd4267 /wd4996 /wd4457" )
endif()

set( LIBVORBIS_MAJOR_VERSION 1 )
set( LIBVORBIS_MINOR_VERSION 3 )
set( LIBVORBIS_PATCH_VERSION 5 )
set( LIBVORBIS_VERSION "${LIBVORBIS_MAJOR_VERSION}.${LIBVORBIS_MINOR_VERSION}.${LIBVORBIS_PATCH_VERSION}" )

set( CMAKE_DEBUG_POSTFIX d )

# Build options
#option( BUILD_SHARED_LIBS "Build shared library" OFF )

if( NOT VORBIS_BUILD_SHARED_LIBS )
	set( VORBIS_BUILD_SHARED_LIBS off )
endif()

set( BUILD_TYPE STATIC )
if( VORBIS_BUILD_SHARED_LIBS )
	set( BUILD_TYPE SHARED )
endif()

#find_package( ogg HINTS ${CMAKE_BINARY_DIR} CONFIG REQUIRED )


set( VORBIS_PUBLIC_HEADERS
    include/vorbis/codec.h
    include/vorbis/vorbisenc.h
    include/vorbis/vorbisfile.h
)

set( VORBIS_HEADERS
    lib/envelope.h
    lib/lpc.h
    lib/lsp.h
    lib/codebook.h
    lib/misc.h
    lib/psy.h
    lib/masking.h
    lib/os.h
    lib/mdct.h
    lib/smallft.h
    lib/highlevel.h
    lib/registry.h
    lib/scales.h
    lib/window.h
    lib/lookup.h
    lib/lookup_data.h
    lib/codec_internal.h
    lib/backends.h
    lib/bitrate.h
)

set( VORBIS_SOURCES
    lib/mdct.c
    lib/smallft.c
    lib/block.c
    lib/envelope.c
    lib/window.c
    lib/lsp.c
    lib/lpc.c
    lib/analysis.c
    lib/synthesis.c
    lib/psy.c
    lib/info.c
    lib/floor1.c
    lib/floor0.c
    lib/res0.c
    lib/mapping0.c
    lib/registry.c
    lib/codebook.c
    lib/sharedbook.c
    lib/lookup.c
    lib/bitrate.c
)

set( VORBISFILE_SOURCES
    lib/vorbisfile.c
)

set( VORBISENC_SOURCES
    lib/vorbisenc.c
)

if( MSVC )
    list( APPEND VORBIS_SOURCES win32/vorbis.def )
    list( APPEND VORBISENC_SOURCES win32/vorbisenc.def )
    list( APPEND VORBISFILE_SOURCES win32/vorbisfile.def )
endif()

include_directories( "include" )
include_directories( "lib" )
include_directories( ${OGG_INCLUDE_DIRS})

add_library( vorbis ${BUILD_TYPE} ${VORBIS_HEADERS} ${VORBIS_SOURCES} )
add_library( vorbisenc ${BUILD_TYPE} ${VORBISENC_SOURCES} )
add_library( vorbisfile ${BUILD_TYPE} ${VORBISFILE_SOURCES} )


target_link_libraries( vorbis ogg )
target_link_libraries( vorbisenc ogg vorbis)
target_link_libraries( vorbisfile ogg vorbis)

###########################################################
# SECTION: Install
###########################################################

set( FOLDER_NAME "${PROJECT_NAME}-${LIBVORBIS_MAJOR_VERSION}.${LIBVORBIS_MINOR_VERSION}.${LIBVORBIS_PATCH_VERSION}")
set( BASE_INSTALL_DIR "${VORBIS_INSTALL_HEADER_BASE}/${FOLDER_NAME}" )

install_headers( "${VORBIS_PUBLIC_HEADERS}" "${BASE_INSTALL_DIR}" )
install_library( vorbis )
install_library( vorbisenc )
install_library( vorbisfile )
