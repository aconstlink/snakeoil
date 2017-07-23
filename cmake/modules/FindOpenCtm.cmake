#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( OPENCTM_FOUND False )

#
# SECTION : Option
#
if( NOT OPENCTM_ENABLE )
    set( OPENCTM_ENABLE OFF )
endif()

option( OPENCTM_ENABLE "Enable OpenCTM SDK" ${OPENCTM_ENABLE} )

if( OPENCTM_ENABLE )
	message( STATUS "[Enabled] : OpenCTM" )
else()
	message( STATUS "[Disabled] : OpenCTM" )
	return()
endif()

set( OPENCTM_DEFINITIONS 
	-DOPENCTM_ENABLE
	)
	
#
# SECTION : Find
#

find_path( OPENCTM_INCLUDE_DIRECTORY 
	NAMES "openctm.h" 
	HINTS $ENV{OPENCTM_INCLUDEDIR} 
	)

if( NOT OPENCTM_INCLUDE_DIRECTORY )
	message( FATAL_ERROR "OpenCTM not found. Please direct CMake to the path of your openctm compilation." )
endif() 

set( OPENCTM_INCLUDE_DIRS ${OPENCTM_INCLUDE_DIRECTORY} )

find_library( OPENCTM_LIBRARY_GENERAL 
	NAMES "openctm" 
	HINTS $ENV{OPENCTM_LIBRARYDIR} 
	)

if( NOT OPENCTM_LIBRARY_GENERAL )
	message( FATAL_ERROR "OpenCTM library not found." )
endif()

set( OPENCTM_LIBRARIES 
	general ${OPENCTM_LIBRARY_GENERAL} 
	)

message( STATUS "[Found] : OpenCTM")

set( OPENCTM_FOUND True )
