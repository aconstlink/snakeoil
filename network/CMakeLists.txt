#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

if( VECTORXL_ENABLE )
    include_directories( ${VECTORXL_INCLUDE_DIRS} ) 
endif()

set( WIN32_SOURCES
    socket/win32/win32_socket_system.h
    socket/win32/win32_socket_system.cpp
    socket/win32/win32_socket_system_creator.cpp
)

set( LINUX_SOURCES 
    socket/linux/linux_socket_system.h
    socket/linux/linux_socket_system.cpp
    socket/linux/linux_socket_system_creator.cpp
    )

#
# sources
#
set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h
    
    utility/transform.h
    
    socket/isocket_system.h
    socket/socket_id.h
    socket/socket_funk.h
    socket/null/null_socket_system.h
    socket/null/null_socket_system.cpp
    socket/socket_system_creator.h
    socket/socket_system_creator.cpp
    
    address/ip4_address.h
    binding/ip4_binding_point.h
    )

if( SNAKEOIL_TARGET_OS_WIN )
    set( SOURCES ${SOURCES} ${WIN32_SOURCES} )
elseif( SNAKEOIL_TARGET_OS_LIN )
    set( SOURCES ${SOURCES} ${LINUX_SOURCES} )
else() 
    message( "No proper socket system available." )
endif()

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_TARGET_OS_WIN )
    set( OS_LIBS Ws2_32.lib )
elseif( SNAKEOIL_TARGET_OS_LIN )
    set( OS_LIBS pthread )
endif()

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_NETWORK_EXPORT
        -DSNAKEOIL_THREAD_IMPORT
        -DSNAKEOIL_LOG_IMPORT
        -DSNAKEOIL_MEMORY_IMPORT
        )
        
endif()

# snakeoil
target_link_libraries( ${CUR_LIB_NAME}
    snakeoil::log
    snakeoil::memory
    snakeoil::thread
)

target_link_libraries( ${CUR_LIB_NAME} ${OS_LIBS} ) 

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
