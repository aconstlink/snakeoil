#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

if( NOT SNAKEOIL_OS_CONFIGURED )
    message( FATAL_ERROR "OS must be configured" )
endif()

option( SNAKEOIL_LIBRARY_BUILD_SHARED "Build shared libraries?" ON )

if( SNAKEOIL_LIBRARY_BUILD_SHARED ) 
    message( STATUS "snakeoil library build type: shared" )
    set( SNAKEOIL_LIBRARY_BUILD_TYPE SHARED )
    # if we build a shared library or an application,
    # all external libs need to be linked.
    set( SNAKEOIL_EXTERNAL_LINKAGE_REQUIRED 1 )

    if( SNAKEOIL_TARGET_OS_WIN )
        add_definitions( -DSNAKEOIL_BUILD_DLL )
    endif()
else()
    message( STATUS "snakeoil library build type: static" )
    set( SNAKEOIL_LIBRARY_BUILD_TYPE STATIC )
    set( SNAKEOIL_EXTERNAL_LINKAGE_REQUIRED 0 )
endif()


