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

    "handle/handle.h"
    "handle/handle.cpp"
   
    "system/system.h"
    "system/system.cpp"    

    "converter/math/string_to_value_converter.hpp"
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )

target_link_libraries( ${CUR_LIB_NAME}
        PUBLIC snakeoil::thread
        PUBLIC snakeoil::log
        PUBLIC snakeoil::memory
        PUBLIC snakeoil::std
    )

    
if( SNAKEOIL_TARGET_OS_LIN )
  target_link_libraries( ${CUR_LIB_NAME}
		stdc++fs
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
