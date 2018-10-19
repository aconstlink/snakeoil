#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h
    dummy.cpp

    container/vector.hpp
    container/list.hpp
    container/map.hpp
    container/stack.hpp

    filesystem/filesystem.hpp
    
    "string/string.hpp"
    "string/utf8.hpp"
    "string/to_string.h"
    "string/split.hpp"
    )

set( SOURCES ${SOURCES} )
so_vs_src_dir( SOURCES )

##########################################################
# configure config.h
configure_file( "${SNAKEOIL_SOURCE_DIR}/std/filesystem/filesystem_config.in.h"
  "${SNAKEOIL_BINARY_DIR}/std/filesystem_config.h" @ONLY )

set( SOURCES_CONFIG ${SNAKEOIL_BINARY_DIR}/std/filesystem_config.h )

set( TO_BE_INCLUDED "${SNAKEOIL_BINARY_DIR}/std" )
##########################################################

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
#add_library( ${CUR_LIB_NAME} INTERFACE ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )
target_include_directories( ${CUR_LIB_NAME}   
        PUBLIC $<BUILD_INTERFACE:${TO_BE_INCLUDED}> )
        #PUBLIC ${TO_BE_INCLUDED} ) 

target_link_libraries( ${CUR_LIB_NAME}
        PUBLIC snakeoil::log
        PUBLIC snakeoil::memory )

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )


