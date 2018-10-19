#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( OS_SPECIFIC_SOURCES )

if( SNAKEOIL_TARGET_OS_WIN AND XAUDIO2_FOUND )

    include_directories( 
        ${XAUDIO2_INCLUDE_DIRECTORY}
    )
    
    add_definitions( ${XAUDIO2_DEFINITIONS} )

    if( SNAKEOIL_COMPILER_MSC )
        # warning C4005 : '__useHeader' : macro redefinition 
        # Note: C4005 seem to be a redefinition bug in the window header files.
        set( SNAKEOIL_UNWANTED_COMPILER_OPTIONS "/wd4005" )
        set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SNAKEOIL_UNWANTED_COMPILER_OPTIONS}" )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SNAKEOIL_UNWANTED_COMPILER_OPTIONS}" )
    endif()

    set( OS_SPECIFIC_SOURCES ${OS_SPECIFIC_SOURCES}
        api/xaudio2/ixaudio2_api.h
        api/xaudio2/xaudio2_api.h
        api/xaudio2/xaudio2_api.cpp
        api/xaudio2/pcm_info.h
        
        api/xaudio2/objects/audio_buffer.h
    )
    set( OS_SPECIFIC_LIBS ${OS_SPECIFIC_LIBS} ${XAUDIO2_LIBRARIES} )
    
endif()

if( SNAKEOIL_TARGET_OS_WIN )
    
    set( OS_SPECIFIC_SOURCES ${OS_SPECIFIC_SOURCES}
        api/wasapi/iwasapi_api.h
        api/wasapi/wasapi_api.h
        api/wasapi/wasapi_api.cpp
    )
    
    add_definitions( -DWASAPI_ENABLE )
    
    set( OS_SPECIFIC_LIBS ${OS_SPECIFIC_LIBS} ole32.lib )
    
endif()

#
# sources
#
set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h
    object.h
    object.cpp
    
    api/iapi.h
    api/api_object.h
    api/api_types.h
    api/api_creator.h
    api/api_creator.cpp
    api/null/null_api.h
    api/null/null_api.cpp
    
    handler/handler_id.h
    handler/handler.h
    handler/handler_handle.h
    
    buffer/ibuffer.h    
    buffer/loopback_buffer.h
    buffer/loopback_buffer.cpp
    buffer/audio_buffer.h
    buffer/audio_buffer.cpp
    
    buffer/pcm/pcm_types.h
    buffer/pcm/ipcm_buffer.h
    buffer/pcm/pcm_buffer.hpp
	buffer/pcm/imex_pcm_wrapper.h
	buffer/pcm/imex_pcm_wrapper.cpp

    ${OS_SPECIFIC_SOURCES}
)

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )

target_link_libraries( ${CUR_LIB_NAME}
    PUBLIC snakeoil::thread
    PUBLIC snakeoil::log
    PUBLIC snakeoil::memory
    PUBLIC snakeoil::std
)

#all 3rd party
target_link_libraries( ${CUR_LIB_NAME}
    PUBLIC ${OS_SPECIFIC_LIBS}
)
    
###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

#install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )

