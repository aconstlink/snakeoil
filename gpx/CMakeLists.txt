#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h
    
    window/iwindow.h
    window/window_id.h
    system/render_system.h
    system/render_system.cpp
    system/render_system_structs.h
    
    technique/technique_id.h
    technique/itechnique.h
    technique/technique.h
    technique/technique.cpp
    technique/technique_state.h
    
    service/iglobal_service.h
    service/ilocal_service.h
    service/service.h
    service/service.cpp
    service/iuser_data.h
    
    plug/iplug.h
    plug/plug.h
    plug/plug_result.h
    plug/iplug_factory.h
    )

set( SOURCES ${SOURCES} )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )

target_link_libraries( ${CUR_LIB_NAME}
  PUBLIC snakeoil::log
  PUBLIC snakeoil::memory
  PUBLIC snakeoil::thread
  PUBLIC snakeoil::gpu
 )

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
