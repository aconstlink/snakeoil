#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( OS_SPECIFIC_SOURCES )

#
# sources
#
set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h
    
    system/audio_system.h
    system/audio_system.cpp
    
    plug/iplug.h
    plug/iplug_factory.h
    plug/plug.h
    plug/plug_result.h
    
    technique/api_id.h
    technique/technique_id.h
    technique/technique_state.h
    technique/audio_technique.h
    technique/audio_technique.cpp
    
    service/iglobal_service.h
    service/ilocal_service.h
    service/iuser_data.h
    service/service.h
    service/service.cpp
)

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_AUDIOX_EXPORT 
        -DSNAKEOIL_IMEX_IMPORT
        -DSNAKEOIL_FLOW_IMPORT
        -DSNAKEOIL_AUDIO_IMPORT
        -DSNAKEOIL_THREAD_IMPORT
        -DSNAKEOIL_LOG_IMPORT
        -DSNAKEOIL_MEMORY_IMPORT
        )
endif()

# snakeoil
target_link_libraries( ${CUR_LIB_NAME}
    snakeoil_imex
    snakeoil_flow
    snakeoil_audio
    snakeoil_thread
    snakeoil_log
    snakeoil_memory
)

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

#install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )

