set( OGG_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
set( OGG_BUILD_SHARED_LIBS ${SNAKEOIL_LIBRARY_BUILD_SHARED} )
add_subdirectory( ogg-1.3.1 )
set_target_properties( ogg PROPERTIES FOLDER "snakeoil/externals/xiph" )

#export ?
