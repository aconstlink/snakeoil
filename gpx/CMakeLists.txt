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
#add_library( ${CUR_LIB_NAME} STATIC ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_GPX_EXPORT
        -DSNAKEOIL_GPU_IMPORT
        -DSNAKEOIL_THREAD_IMPORT
        -DSNAKEOIL_LOG_IMPORT
        -DSNAKEOIL_MEMORY_IMPORT
        )

    # snakeoil
    target_link_libraries( ${CUR_LIB_NAME}
        snakeoil_log
        snakeoil_memory
        snakeoil_thread
        snakeoil_gpu
    )

endif()
