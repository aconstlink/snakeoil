#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES ${SOURCES}
    api.h
    typedefs.h
    protos.h
    
    handle.hpp
    allocator.hpp
    global.h
    global.cpp

    macros/handle.h
    
    utility/uint8_split_merge.h
    utility/uint8_msb_stream.h

    manager/imanager.h
    manager/manager.h
    manager/manager.cpp
    
    guards/malloc_guard.hpp
    
    arena/exp_arena.hpp
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )
    add_definitions( 
        -DSNAKEOIL_MEMORY_EXPORT 
        -DSNAKEOIL_LOG_IMPORT
        )

    target_link_libraries( ${CUR_LIB_NAME}
        snakeoil_log
    )
endif()

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )

