

macro( so_export name )

  #message( ${name} )
  export( TARGETS ${name} APPEND FILE ${CMAKE_BINARY_DIR}/snakeoil-targets2.cmake )

endmacro()
