#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# Finds the OpenAL Soft library
#

find_path( 
	OPENALS_INCLUDE_DIRECTORY 
	NAMES "AL" "ALsoft"
	HINTS "/usr/local/include" "/usr/include" $ENV{OPENALS_INCLUDEDIR} 
	)

if( NOT OPENALS_INCLUDE_DIRECTORY )
	message( FATAL_ERROR "OpenAL not found." )
endif() 

include_directories( ${OPENALS_INCLUDE_DIRECTORY} )

if( NOT SNAKEOIL_EXTERNAL_LINKAGE_REQUIRED ) 
return()
endif()

find_library( OPENALS_COMMON_LIBRARY_DEBUG 
	NAMES "commond" "common" 
	HINTS "/usr/lib64/" "/usr/local/lib64/" $ENV{OPENALS_LIBRARYDIR} )
	
find_library( OPENALS_COMMON_LIBRARY_RELEASE 
	NAMES "common" 
	HINTS "/usr/lib64/" "/usr/local/lib64/" $ENV{OPENALS_LIBRARYDIR} )
	
find_library( OPENALS_LIBRARY_DEBUG 
	NAMES "OpenAL32d" "OpenAL32" 
	HINTS "/usr/lib64/" "/usr/local/lib64/" $ENV{OPENALS_LIBRARYDIR}  )
	
find_library( OPENALS_LIBRARY_RELEASE 
	NAMES "OpenAL32" 
	HINTS "/usr/lib64/" "/usr/local/lib64/" $ENV{OPENALS_LIBRARYDIR} )



