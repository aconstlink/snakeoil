#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES ${SOURCES}
    api.h
    typedefs.h
    protos.h
    result.h

    converter/geometry_binding_converter.h

    #creator/base_scene_creator.h
    #creator/base_scene_creator.cpp
    #creator/full_mld_scene_creator.h
    #creator/full_mld_scene_creator.cpp
    #creator/full_scene_creator.h
    #creator/full_scene_creator.cpp
    #creator/scene_creator_context.h
    #creator/scene_creator_context.cpp

    component/icomponent.h
    component/component.h
    component/component.cpp

    node/node.h
    node/node.cpp
    node/group.h
    node/group.cpp
    node/decorator.h
    node/decorator.cpp
    node/iparent.h
    node/leaf.h
    node/leaf.cpp

    node/camera/camera.h
    node/camera/camera.cpp

    node/group/logic_group.h
    node/group/logic_group.cpp
    node/group/switch_group.h
    node/group/switch_group.cpp

    node/decorator/logic_decorator.h
    node/decorator/logic_decorator.cpp

    node/leaf/logic_leaf.h
    node/leaf/logic_leaf.cpp

    node/render/renderable.h
    node/render/renderable.cpp
    node/render/render_state.h
    node/render/render_state.cpp

    node/transform/transform_3d.h
    node/transform/transform_3d.cpp
    
    node/policy/variable_node_policy.h
    node/policy/variable_node_policy.cpp
    
    visitor/ivisitor.h
    visitor/visitor.h
    visitor/visitor.cpp
    visitor/std_out/std_out.h
    visitor/std_out/std_out.cpp

    #visitor/render/direct_gpx_render.h
    #visitor/render/direct_gpx_render.cpp
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    # snakeoil
    target_link_libraries( ${CUR_LIB_NAME}
        snakeoil::shade
        snakeoil::flow
        snakeoil::gpx
        snakeoil::gpu
        snakeoil::memory
        snakeoil::log
        snakeoil::imex
        snakeoil::geometry
        snakeoil::gfx
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
install_library( ${CUR_LIB_NAME} )
