#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES
    api.h
    result.h
    typedefs.h
    protos.h

    component/trigger_component.h
    component/trigger_component.cpp
    visitor/render_visitor.h
    visitor/render_visitor.cpp
    )

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
