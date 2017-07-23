#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
#
#

set( SNAKEOIL_XLIB_FOUND FALSE )

find_path( XLIB_INCLUDE_DIRECTORY "X11" 
	PATHS "/usr/include" )

if( NOT XLIB_INCLUDE_DIRECTORY )
    message( FATAL_ERROR "Xlib include directory not found.")
    
endif()

include_directories(${XLIB_INCLUDE_DIRECTORY})

if( SNAKEOIL_EXTERNAL_LINKAGE_REQUIRED ) 

	find_library( XLIB_LIBRARY_GENERAL "X11" 
		PATHS "usr/X11/lib" "/usr/lib64/" 
		"/usr/local/lib64/" "/usr/lib/" "/usr/local/lib/")

	set( XLIB_LIBRARIES 
		general ${XLIB_LIBRARY_GENERAL} 
		)
endif()

set( SNAKEOIL_XLIB_FOUND TRUE )


