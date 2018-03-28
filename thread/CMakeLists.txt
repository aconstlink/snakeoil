#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( UNIX_SOURCES
    helper/unix/pthread_helper.h
    )

    set( WIN32_SOURCES 
    helper/win32/win32_helper.h
    )

if( SNAKEOIL_TARGET_OS_WIN OR SNAKEOIL_WIN64 )
set(SOURCES ${SOURCES} ${WIN32_SOURCES})
add_definitions( -DWIN32_LEAN_AND_MEAN )
elseif(SNAKEOIL_TARGET_OS_LIN)
set(SOURCES ${SOURCES} ${UNIX_SOURCES})
endif()

set( SOURCES ${SOURCES}
    api.h
    result.h
    typedefs.h
    protos.h
    global.h
    global.cpp

    primitive/sync_object.hpp
    helper.h
    helper/helper_os.h

    semaphore.hpp
    semaphore_guard.hpp
    mutex.h
    scheduler.h
    scheduler.cpp

    container/vector.hpp
    container/double_buffered/item.hpp
    container/double_buffered/map.hpp
    container/double_buffered/vector.hpp

    task/itask.h
    task/task.h
    task/task.cpp
    task/task_graph.h
    task/task_graph.cpp
    task/tasks.h
    task/tasks.cpp
    task/serial_executor.h
    task/serial_executor.cpp
    task/async_executor.h
    task/async_executor.cpp
    task/itask_scheduler.h
    task/task_scheduler.h
    task/task_scheduler.cpp
        
    command_system/command.h
    command_system/command_list.h
    command_system/command_list.cpp
    command_system/command_system.h
    command_system/command_system.cpp
    command_system/command_system_state.h

    parallel_for.hpp
    range/range_1d.hpp
    
    job/job.h
    job/job_funk.h
    job/job_scheduler.h
    job/job_scheduler.cpp
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_THREAD_EXPORT 
        -DSNAKEOIL_LOG_IMPORT
        -DSNAKEOIL_MEMORY_IMPORT
        )

    # snakeoil
    target_link_libraries( ${CUR_LIB_NAME}
        snakeoil_log
        snakeoil_memory
    )

endif()

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )


