#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES ${SOURCES}
    api.h
    typedefs.h
    protos.h

    permutation/ipermutation.h
    permutation/permutation_table.h
    permutation/permutation_table.cpp

    method/inoise.h
    method/fbm.hpp
    method/value_noise.h
    method/value_noise.cpp
    method/gradient_noise.h
    method/gradient_noise.cpp
    method/perlin_noise.hpp
    method/simplex_noise.hpp
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_NOISE_EXPORT 
        -DSNAKEOIL_MEMORY_IMPORT
        -DSNAKEOIL_LOG_IMPORT
        )

    # snakeoil
    target_link_libraries( ${CUR_LIB_NAME}
        snakeoil_memory
        snakeoil_log
    )

endif()

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
