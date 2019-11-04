#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

if( SNAKEOIL_TARGET_GRAPHICS_OPENGL_33 )
  set( GRAPHICS_SOURCES 
    post/gl33/gl33_post_plug.h
    post/gl33/gl33_post_plug.cpp )
endif()

set( SOURCES
    api.h
    result.h
    typedefs.h
    protos.h
    structs.h

    presentation/presentation.h
    presentation/presentation.cpp
    content/ipage.h
    content/page.h
    content/page.cpp
    content/itransition.h
    content/transition.h
    content/transition.cpp

    content/log/log_page.h
    content/log/log_page.cpp
    content/log/log_transition.h
    content/log/log_transition.cpp

    post/post.h
    post/post.cpp
    post/post_plug_factory.h
    post/post_plug_factory.cpp
    post/post_shared_data.h
    post/protos.h
    )

list( APPEND SOURCES ${GRAPHICS_SOURCES} )

set( SOURCES ${SOURCES} )
so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )

target_link_libraries( ${CUR_LIB_NAME}
  PUBLIC snakeoil::complete
    )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )


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
