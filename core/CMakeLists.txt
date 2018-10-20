#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES 
    "types.hpp"
    "assert.h"
    "boolean.h"
    "checks.h"    
    "break_if.hpp"
    "execute_if.hpp"
    "cast.hpp"
    "execute_result.h"
    
    "macros/common.h"
    "macros/typedef.h"
    "macros/function.h"
    "macros/move.h"

    "enums/types.h"
    "enums/type_traits.hpp"
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} STATIC ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

###########################################################
# SECTION: Build Tree
###########################################################

#so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )


