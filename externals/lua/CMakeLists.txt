

option( LUA_BUILD_AS_DLL "" ${SNAKEOIL_LIBRARY_BUILD_SHARED} )
set( LUA_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
add_subdirectory( lua-5.3.3 )
unset( LUA_BUILD_AS_DLL CACHE )
set_target_properties( lua PROPERTIES FOLDER "snakeoil/externals" )



export( TARGETS lua NAMESPACE snakeoil:: APPEND FILE ${SNAKEOIL_BINARY_DIR}/snakeoil-targets.cmake )
