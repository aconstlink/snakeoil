#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES ${SOURCES}
    api.h
    typedefs.h
    protos.h
    result.h

    slot/islot.h
    slot/iinput_slot.h
    slot/ioutput_slot.h

    slot/input/input_slot.hpp
    slot/input/input_slot.h
    slot/input/input_slot.cpp
    #slot/input/input_slots.h
    #slot/input/input_slots.cpp	


    slot/output/output_slot.hpp
    slot/output/output_slot.h
    slot/output/output_slot.cpp
    #slot/output/output_slots.h
    #slot/output/output_slots.cpp

    node/inode.h
    node/node.h
    node/node.cpp
    node/generic/generic_node.h
    node/generic/generic_node.cpp
    node/variable/variable_node.h
    node/variable/variable_node.cpp
    
    node/logic/logic_node.h
    node/logic/logic_node.cpp
    node/logic/ilogic.h
    
    node/policy/handle.hpp
    node/policy/node_input_policy.h
    node/policy/node_input_policy.cpp
    node/policy/node_output_policy.h
    node/policy/node_output_policy.cpp
    node/connector/connector.h
    node/connector/connector.cpp

    walker/inode_walker.h
    walker/serial_node_walker.h
    walker/serial_node_walker.cpp
    
    data/idata.h
    data/idata.cpp
    data/data.hpp
    data/compatibility_funk.h
    data/generic_funk.h

    variable/ivariable.h
    variable/variable.hpp
    variable/variable_set.h
    variable/variable_set.cpp
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )

target_link_libraries( ${CUR_LIB_NAME}
        PUBLIC snakeoil::thread
        PUBLIC snakeoil::memory
        PUBLIC snakeoil::log  )

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )
