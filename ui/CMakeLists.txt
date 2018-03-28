#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES ${SOURCES}
    api.h
    typedefs.h
    protos.h
    result.h

    structs/mouse_event.h
    structs/inode_listener.h

    component/icomponent.h
    component/layout/row_layout.h
    component/layout/row_layout.cpp
    component/layout/column_layout.h
    component/layout/column_layout.cpp
    component/layout/layer.h
    component/layout/layer.cpp
    component/transform/rect_transform.h
    component/transform/rect_transform.cpp
    component/transform/size_requirement.h
    component/transform/size_requirement.cpp
    component/event/mouse_event.h
    component/event/mouse_event.cpp
    component/text/text.h
    component/text/text.cpp
    component/render/gfx_draw_2d.h
    component/render/gfx_draw_2d.cpp
    component/render/gfx_2d.h
    component/render/gfx_2d.cpp
    component/render/framebuffer_2d.h
    component/render/framebuffer_2d.cpp

    node/node.h
    node/node.cpp
    node/group.h
    node/group.cpp
    node/decorator.h
    node/decorator.cpp
    node/iparent.h
    node/leaf.h
    node/leaf.cpp

    node/group/logic_group.h
    node/group/logic_group.cpp
    node/group/switch_group.h
    node/group/switch_group.cpp

    node/decorator/logic_decorator.h
    node/decorator/logic_decorator.cpp

    node/leaf/logic_leaf.h
    node/leaf/logic_leaf.cpp
    
    node/component/variable_node_component.h
    node/component/variable_node_component.cpp
    node/component/components_component.h
    node/component/components_component.cpp
    
    visitor/ivisitor.h
    visitor/visitor.h
    visitor/visitor.cpp
    visitor/log/log.h
    visitor/log/log.cpp
    visitor/update/layout_updater.h
    visitor/update/layout_updater.cpp
    visitor/render/image_render_2d.h
    visitor/render/image_render_2d.cpp
    visitor/render/text_render_2d.h
    visitor/render/text_render_2d.cpp
    visitor/render/line_render_2d.h
    visitor/render/line_render_2d.cpp
    visitor/render/rect_render_2d.h
    visitor/render/rect_render_2d.cpp
    visitor/render/render_2d.h
    visitor/render/render_2d.cpp
    visitor/render/gfx_draw_2d.h
    visitor/render/gfx_draw_2d.cpp
    
    visitor/event/mouse_event_tester.h
    visitor/event/mouse_event_tester.cpp
    visitor/debug/draw_leafs.h
    visitor/debug/draw_leafs.cpp
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_UI_EXPORT
        )

    # snakeoil
    target_link_libraries( ${CUR_LIB_NAME}
        snakeoil_shade
        snakeoil_flow
        snakeoil_gpx
        snakeoil_gpu
        snakeoil_memory
        snakeoil_log
        snakeoil_imex
        snakeoil_geometry
        snakeoil_gfx
        snakeoil_device
    )
	
endif()

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
