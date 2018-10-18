#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES ${SOURCES}
    api.h
    typedefs.h
    protos.h
    result.h
    
    iproperty.h
    properties/group_property.h
    properties/group_property.cpp
    properties/generic_property.hpp
    properties/single_choice_property.h
    properties/single_choice_property.cpp
    properties/multi_choice_property.h
    properties/multi_choice_property.cpp
    properties/decorator_property.h
    properties/decorator_property.cpp
    properties/bool_property.hpp
    properties/string_property.hpp
    properties/numeric_property.hpp
    
    property_sheet.h
    property_sheet.cpp
    property_sheet.cpp   
    
    ivisitor.h
    visitor/property_construction_visitor.h
    visitor/property_construction_visitor.cpp
    visitor/std_out_visitor.h
    visitor/std_out_visitor.cpp
)

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_PROPERTY_EXPORT
        -DSNAKEOIL_MEMORY_IMPORT
        -DSNAKEOIL_LOG_IMPORT
        )

    # snakeoil
    target_link_libraries( ${CUR_LIB_NAME}
        snakeoil::memory
        snakeoil::log
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
