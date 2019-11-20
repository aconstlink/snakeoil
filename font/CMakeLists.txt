#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

include_directories( ${STB_INCLUDE_DIRS} )

set( FREETYPE_SOURCES
    #"driver/freetype/freetype_driver.h"
    #"driver/freetype/freetype_driver.cpp"
    #"driver/freetype/items/freetype_face.h"
)

set( STB_SOURCES 
    "glyph_atlas/stb/stb_glyph_atlas_creator.h"
    "glyph_atlas/stb/stb_glyph_atlas_creator.cpp"
)

set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h
    
    glyph_atlas/glyph_atlas.h
    glyph_atlas/glyph_atlas.cpp
    
    ${STB_SOURCES}
    )

if( FREETYPE_ENABLE )    
    set( SOURCES ${SOURCES} ${FREETYPE_SOURCES} )
endif()

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )

target_link_libraries( ${CUR_LIB_NAME}
  PRIVATE stb
  PUBLIC snakeoil::log
  PUBLIC snakeoil::memory
  PUBLIC snakeoil::io
  PUBLIC snakeoil::gpu
  PUBLIC snakeoil::gpx
  )

# freetype
if( FREETYPE_ENABLE )
  target_link_libraries( ${CUR_LIB_NAME}
    PUBLIC freetype
    )
endif()

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
