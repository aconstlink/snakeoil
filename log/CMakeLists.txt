#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES 
    "api.h"
    "result.h"
    "protos.h"
    "typedefs.h"
    
    "global.h"
    "global.cpp"
    "log_level.h"

    "system/isystem.h"
    "system/system.h"
    "system/system.cpp"
    
    "logger/logger.h"
    "logger/std_cerr_logger.h"
    "logger/std_cerr_logger.cpp"
    "logger/store_logger.h"
    "logger/store_logger.cpp"
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} ) 

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME_SHORT} )


