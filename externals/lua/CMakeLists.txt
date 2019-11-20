
set( LUA_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
add_subdirectory( lua-5.3.3 )
set( LUA_VERSION 5.3.3 )
set( LUA_ENABLE ON )
set_target_properties( lua PROPERTIES FOLDER "snakeoil/externals" )

export( TARGETS lua NAMESPACE snakeoil:: APPEND FILE ${SNAKEOIL_BINARY_DIR}/snakeoil-targets.cmake )
