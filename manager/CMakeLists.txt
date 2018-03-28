#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h

    base_manager.hpp
    
    system/system.h
    system/system.cpp

    registry/manager_registry.h
    registry/manager_registry.cpp

    code/icode_manager.h
    code/code_manager.h
    code/code_manager.cpp

    camera/camera_manager.h
    camera/camera_manager.cpp
    
    graphics/shader/iprogram_manager.h
    graphics/shader/program_manager.h
    graphics/shader/program_manager.cpp
    graphics/shader/ishader_manager.h
    graphics/shader/shader_manager.h
    graphics/shader/shader_manager.cpp
    graphics/shader/core_shader_manager.hpp
    graphics/geometry/geometry_manager.h
    graphics/geometry/geometry_manager.cpp
    graphics/geometry/buffer_creator.h
    graphics/geometry/buffer_creator.cpp
    graphics/geometry/buffer_creator_mesh.cpp
    graphics/geometry/buffer_creator_wire.cpp

    graphics/imagery/image_manager.h
    graphics/imagery/image_manager.cpp
    graphics/imagery/iframebuffer_manager.h
    graphics/imagery/framebuffer_manager.h
    graphics/imagery/framebuffer_manager.cpp
    graphics/imagery/texture_manager.h
    graphics/imagery/texture_manager.cpp
    graphics/imagery/image_creator.h
    graphics/imagery/image_creator.cpp

    graphics/variable/ivariable_manager.h
    graphics/variable/variable_manager.h
    graphics/variable/variable_manager.cpp
    
    spline/spline_manager.h
    spline/spline_manager.cpp
    
    flow/flow_node_manager.h
    flow/flow_node_manager.cpp
    )

set( SOURCES ${SOURCES} )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_MANAGER_EXPORT
        -DSNAKEOIL_GPX_IMPORT
        -DSNAKEOIL_GPU_IMPORT
        -DSNAKEOIL_THREAD_IMPORT 
        -DSNAKEOIL_LOG_IMPORT
        -DSNAKEOIL_MEMORY_IMPORT
        )

    # snakeoil
    target_link_libraries( ${CUR_LIB_NAME}
        snakeoil_gpx
        snakeoil_gpu
        snakeoil_thread
        snakeoil_log
        snakeoil_memory
    )

endif()


###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
