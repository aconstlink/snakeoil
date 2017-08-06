#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES ${SOURCES}
    api.h
    typedefs.h
    protos.h
    result.h

    ivariable.h
    variable.hpp
    variable_set/variable_set.h
    variable_set/variable_set.cpp
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_VARIABLE_EXPORT 
        -DSNAKEOIL_MEMORY_IMPORT
        -DSNAKEOIL_LOG_IMPORT
        )

    target_link_libraries( ${CUR_LIB_NAME}
        snakeoil_memory
        snakeoil_log
    )

endif()

