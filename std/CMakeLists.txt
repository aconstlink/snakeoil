#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h

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

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )

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


