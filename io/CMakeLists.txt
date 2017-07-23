#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES 
    "api.h"
    "result.h"
    "protos.h"
    "typedefs.h"
    "io.h"
    "io.cpp"
    
    "handle/handle.h"
    "handle/handle.cpp"
   
    "system/system.h"
    "system/system.cpp"    

    "converter/math/string_to_value_converter.hpp"
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_IO_EXPORT 
        -DSNAKEOIL_THREAD_IMPORT
        -DSNAKEOIL_LOG_IMPORT
        -DSNAKEOIL_MEMORY_IMPORT
        )

    # snakeoil
    target_link_libraries( ${CUR_LIB_NAME}
        snakeoil_thread
        snakeoil_log
        snakeoil_memory
    )
    
    if( SNAKEOIL_TARGET_OS_LIN )
		target_link_libraries( ${CUR_LIB_NAME}
			stdc++fs
		)
    endif()

endif()
