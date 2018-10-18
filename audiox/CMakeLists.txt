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
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )

target_link_libraries( ${CUR_LIB_NAME}
    PUBLIC snakeoil::imex
    PUBLIC snakeoil::flow
    PUBLIC snakeoil::audio
    PUBLIC snakeoil::thread
    PUBLIC snakeoil::log
    PUBLIC snakeoil::memory )

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

#install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )

