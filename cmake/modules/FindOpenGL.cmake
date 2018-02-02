#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

option( GL_ERROR_CHECK "Using glGetError?" ON )

if( GL_ERROR_CHECK )
    message(STATUS "FindOpenGL: GL error will be checked during run-time.")
    add_definitions( -DSNAKEOIL_OPT_GL_CHECK_ERROR )
endif()

if( SNAKEOIL_TARGET_OS_WIN )

    if( SNAKEOIL_TARGET_OS_WIN7 )

        set( OPENGL_SEARCH_LIB_PATHS 
            "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Lib/x64"
            "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/x64"    )

    elseif( SNAKEOIL_TARGET_OS_WIN81 OR SNAKEOIL_TARGET_OS_WIN10 )

        set( OPENGL_SEARCH_LIB_PATHS 
            "C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64"    )
        
    endif()

elseif( SNAKEOIL_TARGET_OS_LIN )
    set( OPENGL_SEARCH_LIB_PATHS "/usr/lib" "usr/lib64" )
endif()


if( NOT OPENGL_PATH_HINT )
    set( OPENGL_PATH_HINT "${CMAKE_CURRENT_LIST_DIR}/../../externals/khronos/include" )
endif()

find_path( GL_INCLUDE_DIRECTORY 
    NAMES "GL/glcorearb.h" 
    HINTS
    ${OPENGL_PATH_HINT}
    "usr/local/include" 
    "usr/include" 
    $ENV{OPENGL_INCLUDEDIR}
    )

if( NOT GL_INCLUDE_DIRECTORY )
    message( FATAL_ERROR "OpenGL include directory required" ) 
endif()

set( OPENGL_INCLUDE_DIRS 
    ${GL_INCLUDE_DIRECTORY} 
    )

set( SNAKEOIL_GL_FOUND TRUE )

if( NOT ${SNAKEOIL_REQUIRES_LINKAGE} ) 
return()
endif()

find_library( GL_LIBRARY 
    NAMES "GL" "OpenGL32"
    HINTS ${OPENGL_SEARCH_LIB_PATHS} $ENV{OPENGL_LIBRARYDIR} 
    )

find_library( GL_GLU_LIBRARY 
    NAMES "GLU" "GLU32"
    HINTS ${OPENGL_SEARCH_LIB_PATHS} $ENV{OPENGL_LIBRARYDIR} 
    )

if( NOT GL_LIBRARY OR NOT GL_GLU_LIBRARY )
message( FATAL_ERROR "OpenGL Library required" )
endif()

set( OPENGL_LIBRARIES
    general ${GL_LIBRARY}
    general ${GL_GLU_LIBRARY}
)


