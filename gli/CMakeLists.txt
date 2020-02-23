#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( GL_SOURCES
    gl/gl.h
    gl/gl.cpp
)

set( WGL_SOURCES
    wgl/wgl.h
    wgl/wgl.cpp
)

set( GLX_SOURCES
    glx/glx.h
    glx/glx.cpp
)

set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h

    ${GL_SOURCES}
)

if( SNAKEOIL_TARGET_OS_WIN )
    set( SOURCES ${SOURCES} ${WGL_SOURCES} )
    add_definitions( -DWIN32_LEAN_AND_MEAN )
elseif( SNAKEOIL_TARGET_OS_LIN )
    set( SOURCES ${SOURCES} ${GLX_SOURCES} )
endif()

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )

target_link_libraries( ${CUR_LIB_NAME}
  PUBLIC opengl 
  PUBLIC khronos_glheader
  PUBLIC snakeoil::std
  PUBLIC snakeoil::log
  PUBLIC snakeoil::memory
)

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
