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

    node/render/renderable.h
    node/render/renderable.cpp
    node/render/render_state.h
    node/render/render_state.cpp

    node/transform/transform_2d.h
    node/transform/transform_2d.cpp
    node/transform/transform_3d.h
    node/transform/transform_3d.cpp
    
    node/component/variable_node_component.h
    node/component/variable_node_component.cpp

    node/layout/area_2d.h
    node/layout/area_2d.cpp
    node/layout/column_layout.h
    node/layout/column_layout.cpp
    node/layout/row_layout.h
    node/layout/row_layout.cpp
    node/layout/layer_2d.h
    node/layout/layer_2d.cpp
    node/content/fill_area_2d.h
    node/content/fill_area_2d.cpp
    node/content/image_2d.h
    node/content/image_2d.cpp
    node/content/text.h
    node/content/text.cpp
    node/event/input_event.h
    node/event/input_event.cpp

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
    visitor/event/mouse_event_tester.h
    visitor/event/mouse_event_tester.cpp
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

