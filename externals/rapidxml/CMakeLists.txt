set( RAPIDXML_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
add_subdirectory( rapidxml-1.13 )

add_library( rapidxml INTERFACE )
target_include_directories( rapidxml INTERFACE "./rapidxml-1.13/include" )

export( TARGETS rapidxml NAMESPACE snakeoil:: APPEND FILE ${SNAKEOIL_BINARY_DIR}/snakeoil-targets.cmake )
