#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

include_directories(
    ${RAPIDXML_INCLUDE_DIRS}
    ${UTFCPP_INCLUDE_DIRS}
    ${STB_INCLUDE_DIRS}
    ${RAPIDJSON_INCLUDE_DIRS}
)

if( SNAKEOIL_COMPILER_MSC )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${STBI_DISABLE_WARNINGS_MSC}" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STBI_DISABLE_WARNINGS_MSC}" )
elseif( SNAKEOIL_COMPILER_GNU )
    # todo :(
endif()

set( SNAKEOIL_SOURCES
    snakeoil/fragment/ifragment_file_parser.h
    snakeoil/fragment/fragment_file_content.h
    snakeoil/fragment/xml/xml_fragment_file_parser.h
    snakeoil/fragment/xml/xml_fragment_file_parser.cpp
    snakeoil/spritesheet/spritesheet_parser.h
    snakeoil/spritesheet/spritesheet_parser.cpp
    snakeoil/spritesheet/spritesheet.h
    snakeoil/world/world.h
    snakeoil/world/world_parser.h
    snakeoil/world/world_parser.cpp
)

set( SOURCES
    api.h
    result.h
    typedefs.h
    protos.h
    global.h
    global.cpp
    sync_object.h
    sync_object.cpp

    enums/image_file_formats.h
    enums/mesh_file_formats.h
    enums/scene_file_formats.h

    enums/shader_types.h
    enums/operation_types.h
    enums/pixel_formats.h
    enums/camera_types.h

    camera/icamera.h
    camera/generic_camera.cpp
    camera/generic_camera.h
    camera/ilens.h
    camera/pinhole_lens.h
    camera/pinhole_lens.cpp

    image/iimage.h
    image/image.h
    image/image.cpp
    image/deduce_image_format.h
    image/deduce_image_format.cpp

    audio/iaudio.h
    audio/audio_formats.h
    audio/pcm_audio.hpp
    audio/pcm_buffer_types.h
    
    spline/ispline.h
    spline/linear_spline.h
    spline/linear_spline.cpp
    spline/cubic_spline.h
    spline/cubic_spline.cpp
    spline/bezier_spline.h
    spline/bezier_spline.cpp
    
    shader/shader.h
    shader/vertex_shader.h
    shader/pixel_shader.h
    shader/effect.h
    
    manager/base_resource_manager.hpp
    manager/resource_manager.hpp
    manager/asset_manager.h
    manager/asset_manager.cpp
    manager/mesh_manager.h
    manager/mesh_manager.cpp
    manager/image_manager.h
    manager/image_manager.cpp
    manager/graph_manager.h
    manager/graph_manager.cpp
    manager/camera_manager.h
    manager/camera_manager.cpp
    manager/manager_registry.h
    manager/manager_registry.cpp
    manager/spline_manager.h
    manager/spline_manager.cpp

    manager/audio_manager.h
    manager/audio_manager.cpp
    
    manager/shade_manager/material_manager.h
    manager/shade_manager/material_manager.cpp
    manager/shade_manager/vertex_shader_manager.h
    manager/shade_manager/vertex_shader_manager.cpp
    manager/shade_manager/pixel_shader_manager.h
    manager/shade_manager/pixel_shader_manager.cpp
    
    module/iaudio_module.h
    module/iimage_module.h
    module/iscene_module.h
    module/imesh_module.h
    
    module/obj/obj_module.h
    module/obj/obj_module.cpp
    module/obj/obj_module_import.cpp
    module/obj/obj_module_export.cpp
    module/obj/obj_export_context.h
    module/obj/obj_export_context.cpp
    module/obj/obj_document.h
    
    module/stb/stb_module.h
    module/stb/stb_module.cpp
    module/stb/stb_module_import.cpp
    module/stb/stb_module_export.cpp
    
    module/module_registry.h
    module/module_registry.cpp
    
    module/dae/dae_module.h
    module/dae/dae_module.cpp
    module/dae/dae_module_import.cpp
    module/dae/dae_module_export.cpp
    module/dae/dae_export_context.h
    module/dae/dae_export_context.cpp
    module/dae/elements/dae_document.h
    module/dae/elements/dae_node.h
    module/dae/elements/dae_visitor.h
    module/dae/elements/dae_nodes.h
    module/dae/elements/dae_protos.h
    
    module/wav/wav_module.h
    module/wav/wav_module.cpp
    module/wav/wav_module_import.cpp
    module/wav/wav_module_export.cpp
    
    node/iparent.h
    node/node.h
    node/node.cpp
    node/group.h
    node/group.cpp
    node/leaf.h
    node/leaf.cpp
    node/decorator.h
    node/decorator.cpp

    node/leaf/logic_leaf.h
    node/leaf/logic_leaf.cpp

    node/decorator/imported_from.h
    node/decorator/imported_from.cpp
    node/decorator/transform_3d.h
    node/decorator/transform_3d.cpp
    node/decorator/logic_decorator.h
    node/decorator/logic_decorator.cpp

    node/group/logic_group.h
    node/group/logic_group.cpp

    node/material/material_asset.h
    node/material/material_asset.cpp

    node/effect/effect_asset.h
    node/effect/effect_asset.cpp

    node/attribute/geometry_binding.h
    node/attribute/camera_binding.h
    node/attribute/trafo_binding.h

    node/mesh/mesh.h
    node/mesh/mesh.cpp
    node/mesh/mesh_asset.h
    node/mesh/mesh_asset.cpp

    node/image/images.h
    node/image/images.cpp
    node/image/image_asset.h
    node/image/image_asset.cpp

    node/light/light.h
    node/light/light.cpp

    node/camera/camera_asset.h
    node/camera/camera_asset.cpp

    node/spline/spline_asset.h
    node/spline/spline_asset.cpp

    system/isystem.h
    system/system.h
    system/system.cpp
    system/system_default.cpp

    property/property_map.h
    property/property_map.cpp

    ${SNAKEOIL_SOURCES}
    )

set( EXTERNAL_LIBRARIES )

if( OPENCTM_ENABLE )
    set( OPENCTM_SOURCES
        module/ctm/ctm_module.h
        module/ctm/ctm_module.cpp
        module/ctm/ctm_module_import.cpp
        module/ctm/ctm_module_export.cpp
        module/ctm/ctm_helper.h
        )
    set( SOURCES ${SOURCES} ${OPENCTM_SOURCES} )

    include_directories( ${OPENCTM_INCLUDE_DIRS} )
    add_definitions( ${OPENCTM_DEFINITIONS} )

    set( EXTERNAL_LIBRARIES
        ${EXTERNAL_LIBRARIES}
        ${OPENCTM_LIBRARIES}
        )
endif()

set( SOURCES ${SOURCES} )
so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )

target_link_libraries( ${CUR_LIB_NAME}
        PUBLIC snakeoil::property
        PUBLIC snakeoil::shade
        PUBLIC snakeoil::geometry
        PUBLIC snakeoil::log
        PUBLIC snakeoil::memory
        PUBLIC snakeoil::io
        PUBLIC snakeoil::thread
    )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    #all 3rd party
    target_link_libraries( ${CUR_LIB_NAME}
        PUBLIC ${EXTERNAL_LIBRARIES} )

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
