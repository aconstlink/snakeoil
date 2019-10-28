
set( SUBDIRS
    "presentation"
)
foreach( _SUBDIR ${SUBDIRS} )
    set( CUR_LIB_NAME_SHORT "${_SUBDIR}" )
    set( CUR_LIB_NAME "${_SUBDIR}" )
    string(TOUPPER ${CUR_LIB_NAME} CUR_LIB_NAME_UPPER)
    set( CUR_DIR_NAME "${_SUBDIR}" )
    set( CUR_LIB_NAME_RAW "${_SUBDIR}" )
    set( CUR_ALIAS_NAME "${PROJECT_NAME}::${_SUBDIR}" )
    set( CUR_FILE_NAME "${PROJECT_NAME}_${_SUBDIR}" )
    add_subdirectory(${_SUBDIR})

    set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )
    set_target_properties( ${CUR_LIB_NAME} PROPERTIES OUTPUT_NAME ${CUR_FILE_NAME})
    set_target_properties( ${CUR_LIB_NAME} PROPERTIES CXX_STANDARD ${SNAKEOIL_CXX_STANDARD} )
    if( SNAKEOIL_LIBRARY_BUILD_SHARED )
      target_compile_options( ${CUR_LIB_NAME} 
        PRIVATE -DSNAKEOILX_${CUR_LIB_NAME_UPPER}_EXPORT 
        INTERFACE -DSNAKEOILX_${CUR_LIB_NAME_UPPER}_IMPORT )    
    endif()
    target_compile_options( ${CUR_LIB_NAME} PRIVATE ${SNAKEOIL_COMPILER_OPTIONS_PRIVATE})
    target_include_directories( ${CUR_LIB_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../.." )

    set_target_properties( ${CUR_LIB_NAME} PROPERTIES FOLDER "snakeoil/sox" )
    
    target_link_libraries( complete INTERFACE ${CUR_LIB_NAME} )
endforeach()
