

macro( so_export name )

  #message( ${name} )
  export( TARGETS ${name} NAMESPACE snakeoil:: APPEND FILE ${SNAKEOIL_BINARY_DIR}/snakeoil-targets.cmake )

endmacro()

macro( so_export_clear )

file( REMOVE ${SNAKEOIL_BINARY_DIR}/snakeoil-targets.cmake )

endmacro()
