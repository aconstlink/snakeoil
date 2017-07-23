#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# Finds the intel tbb library
#

find_path( INTELTBB_INCLUDE_DIRECTORY 
	NAMES "tbb" 
	HINTS "/usr/local/include" "/usr/include" $ENV{INTELTBB_INCLUDEDIR} )


if( NOT INTELTBB_INCLUDE_DIRECTORY )
	message( FATAL_ERROR "IntelTBB not found. Please direct CMake to the path of your tbb compilation." )
endif() 

include_directories(${INTELTBB_INCLUDE_DIRECTORY} )

set( TBB_FOUND TRUE )

if( NOT SNAKEOIL_EXTERNAL_LINKAGE_REQUIRED )
return() 
endif()

find_library( TBB_LIBRARY_DEBUG "tbb_debug" 
	HINTS "/usr/lib64/" "/usr/local/lib64/" $ENV{INTELTBB_LIBRARYDIR} )
	
find_library( TBB_LIBRARY_RELEASE "tbb" 
	HINTS "/usr/lib64/" "/usr/local/lib64/" $ENV{INTELTBB_LIBRARYDIR} ) 

