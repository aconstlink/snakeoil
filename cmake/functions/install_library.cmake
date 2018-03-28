#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

function( install_library LIB_NAME )

    if( SNAKEOIL_LIBRARY_BUILD_SHARED )
        install( TARGETS ${LIB_NAME} 
            RUNTIME DESTINATION "bin"
            LIBRARY DESTINATION "lib" 
            ARCHIVE DESTINATION "lib" )
    else()
        install( TARGETS ${LIB_NAME} 
            DESTINATION "lib" )
    endif()

endfunction()
