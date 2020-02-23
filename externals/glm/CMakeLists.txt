
add_subdirectory( glm )

if( TARGET glm_shared )
  set_target_properties( glm_shared PROPERTIES FOLDER "snakeoil/externals/glm" )
endif()

if( TARGET glm_static )
  set_target_properties( glm_static PROPERTIES FOLDER "snakeoil/externals/glm" )
endif()
