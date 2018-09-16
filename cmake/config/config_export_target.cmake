

macro( so_export name )

  #message( ${name} )
  export( TARGETS ${name} APPEND FILE ${CMAKE_BINARY_DIR}/snakeoil-targets.cmake )

endmacro()

macro( so_export_clear )

file( REMOVE ${CMAKE_BINARY_DIR}/snakeoil-targets.cmake )

endmacro()
