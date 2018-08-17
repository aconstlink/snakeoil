#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

if( SNAKEOIL_TARGET_GRAPHICS_OPENGL )
    include_directories(
        ${OPENGL_INCLUDE_DIRS}
    )
endif()

set( SOURCES 
    api.h
    protos.h
    result.h
    typedefs.h
    
    application/appx_application.h
    application/appx_application.cpp
    
    system/states.h
    system/appx_system.h
    system/appx_system.cpp
    system/init_data.h
    system/update_data.h
    system/render_data.h
    system/iappx_context.h
    system/appx_context.h
    system/appx_context.cpp
    system/iwindow_state_informer.h
    system/window_state_informer.h
    system/window_state_informer.cpp
    system/event_data.h
    
    app/iapp.h
    app/app.h
    app/app.cpp
    
    )
    
so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_APPX_EXPORT
        -DSNAKEOIL_APPLICATION_IMPORT
        -DSNAKEOIL_IMEX_IMPORT
        -DSNAKEOIL_AUDIOX_IMPORT
        -DSNAKEOIL_AUDIO_IMPORT
        -DSNAKEOIL_GPX_IMPORT
        -DSNAKEOIL_GPU_IMPORT
        -DSNAKEOIL_DEVICE_IMPORT
        -DSNAKEOIL_IO_IMPORT
        -DSNAKEOIL_THREAD_IMPORT
        -DSNAKEOIL_LOG_IMPORT
        -DSNAKEOIL_MEMORY_IMPORT
        )
        
    # snakeoil
    target_link_libraries( ${CUR_LIB_NAME}
        snakeoil_application
        snakeoil_imex
        snakeoil_audiox
        snakeoil_audio
        snakeoil_gpx
        snakeoil_gpu
        snakeoil_thread
        snakeoil_log
        snakeoil_memory
        snakeoil_device
        snakeoil_io
    )
        
    #3rd party
    #target_link_libraries( ${CUR_LIB_NAME}	
    #)

    #win 3rd party
    if( ${SNAKEOIL_TARGET_OS_WIN} )
        target_link_libraries( ${CUR_LIB_NAME}
            general ${XINPUT_LIBRARY_GENERAL}
        )
    endif()
    endif()

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
