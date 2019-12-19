set( STB_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals/stb-2.31" )
add_subdirectory( stb-2.31 )

add_library( stb INTERFACE )
target_include_directories( stb INTERFACE "./stb-2.31/include" )

if( SNAKEOIL_COMPILER_MSC )
  target_compile_options( stb INTERFACE /wd4267 /wd4996 )
endif()

export( TARGETS stb NAMESPACE snakeoil:: APPEND FILE ${SNAKEOIL_BINARY_DIR}/snakeoil-targets.cmake )
