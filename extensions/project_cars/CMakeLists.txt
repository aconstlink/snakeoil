#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES ${SOURCES}
    api.h
    typedefs.h
    structs.h
    udp_receiver.h
    udp_receiver.cpp    
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES FOLDER "extensions" )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )
    add_definitions( 
        -DSNAKEOIL_PROJECT_CARS_EXPORT
        -DSNAKEOIL_NETWORK_IMPORT
        -DSNAKEOIL_MEMORY_IMPORT 
        -DSNAKEOIL_LOG_IMPORT
        )

    target_link_libraries( ${CUR_LIB_NAME}
        snakeoil_network
        snakeoil_memory
        snakeoil_log
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

