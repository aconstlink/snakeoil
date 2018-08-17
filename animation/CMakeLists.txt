#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

##################################################################
#
#
##################################################################

set( SOURCES ${SOURCES}
    api.h
    typedefs.h
    protos.h
    result.h
    evaluation_result.h

    keyframe/keyframe.hpp
    keyframe_sequence/linear_keyframe_sequence.hpp
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_ANIMATION_EXPORT
        -DSNAKEOIL_SPLINE_IMPORT
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
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
