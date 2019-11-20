set( STB_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals/stb-2.31" )
add_subdirectory( stb-2.31 )

add_library( stb INTERFACE )
target_include_directories( stb INTERFACE "./stb-2.31/include" )

export( TARGETS stb NAMESPACE snakeoil:: APPEND FILE ${SNAKEOIL_BINARY_DIR}/snakeoil-targets.cmake )
