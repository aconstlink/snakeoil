#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES ${SOURCES}
    api.h
    typedefs.h
    protos.h
    result.h
    type.h
    variable.h
    texture_type.h
    buffer_type.h
    
    code/icode.h
    code/code.h
    code/code.cpp
    code/code_type.h
    code/ascii_code.h
    code/ascii_code.cpp
    code/version.h
    code/shader_type.h

    funk/ifunk.h
    funk/math_funk.h
    funk/math_funk.cpp

    slot/islot.h
    slot/slot_group.h
    slot/slot_group.cpp
    slot/iinput_slot.h
    slot/ioutput_slot.h
    slot/input_slot.h
    slot/input_slot.cpp
    slot/output_slot.h
    slot/output_slot.cpp
    slot/input_slots.h
    slot/input_slots.cpp
    slot/output_slots.h
    slot/output_slots.cpp	
    slot/inout_slot.h
    slot/inout_slot.cpp
    slot/iconnector.h
    slot/connector_registry.h
    slot/connector_registry.cpp

    tree/node.h
    tree/node.cpp
    tree/group_node.h
    tree/group_node.cpp
    tree/leaf_node.h
    tree/leaf_node.cpp
    tree/funk_node.h
    tree/funk_node.cpp
    tree/funk/math_funk_node.h
    tree/funk/math_funk_node.cpp
    tree/funk/texture_funk_node.h
    tree/funk/texture_funk_node.cpp
    tree/funk/generic_funk_node.h
    tree/funk/generic_funk_node.cpp
    tree/leaf/arg_node.h
    tree/leaf/arg_node.cpp

    graph/node.h
    graph/node.cpp
    graph/group.h
    graph/group.cpp
    graph/generic_funk_node.h
    graph/generic_funk_node.cpp	
        
    graph/output_node/color_output_node.h
    graph/output_node/color_output_node.cpp
    graph/input_node/geometry_input_node.h
    graph/input_node/geometry_input_node.cpp
    graph/input_node/texture_input_node.h
    graph/input_node/texture_input_node.cpp
    graph/input_node/value_input_node.h
    graph/input_node/value_input_node.cpp

    generator/imaterial_generator.h
    generator/ieffect_generator.h
    generator/glsl/generator_properties.h
    generator/glsl/effect_generator.h
    generator/glsl/effect_generator.cpp
    generator/glsl/mld_effect_generator.h
    generator/glsl/mld_effect_generator.cpp
    generator/glsl_es/generator_properties.h
    generator/glsl_es/effect_generator.h
    generator/glsl_es/effect_generator.cpp
    generator/glsl_es/mld_effect_generator.h
    generator/glsl_es/mld_effect_generator.cpp

    kernel/ikernel.h
    kernel/kernel.h
    kernel/kernel.cpp
    kernel/math_kernel.h
    kernel/math_kernel.cpp
    
    material/imaterial.h
    material/material.h
    material/material.cpp
    material/binding/enums.h
    material/binding/bindings.h    
    material/standard_material.h
    material/standard_material.cpp
    material/effect_material.h
    material/effect_material.cpp
    
    shader/ieffect.h
    shader/effect.h
    shader/effect.cpp
    shader/ishader.h
    shader/shader.h
    shader/shader.cpp
    shader/vertex_shader.h
    shader/vertex_shader.cpp
    shader/pixel_shader.h
    shader/pixel_shader.cpp
    shader/binding/buffer_binding.h
    shader/binding/geometry_binding.h
    shader/binding/camera_binding.h
    shader/binding/object_binding.h
    shader/binding/texture_binding.h
    shader/binding/icustom_binding.h
    shader/binding/mld_bindings.h
    shader/policy/custom_bindings_policy.h
    shader/policy/custom_bindings_policy.cpp
)

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

	add_definitions( 
		-DSNAKEOIL_SHADE_EXPORT 
		#-DSNAKEOIL_GPX_IMPORT
		#-DSNAKEOIL_GPU_IMPORT
		-DSNAKEOIL_MEMORY_IMPORT
		-DSNAKEOIL_LOG_IMPORT
		)
	
	# snakeoil
	target_link_libraries( ${CUR_LIB_NAME}
		#snakeoil_gpx
		#snakeoil_gpu
		snakeoil_memory
		snakeoil_log
	)
	
endif()

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
