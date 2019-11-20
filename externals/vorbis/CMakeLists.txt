set( VORBIS_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
set( VORBIS_BUILD_SHARED_LIBS ${SNAKEOIL_LIBRARY_BUILD_SHARED} )

add_subdirectory( vorbis-1.3.5 )
set_target_properties( vorbis PROPERTIES FOLDER "snakeoil/externals/xiph" )
set_target_properties( vorbisenc PROPERTIES FOLDER "snakeoil/externals/xiph" )
set_target_properties( vorbisfile PROPERTIES FOLDER "snakeoil/externals/xiph" )

#exports ?
