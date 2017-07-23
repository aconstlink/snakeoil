#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# Finds the free image library
#

find_path( FREEIMAGE_INCLUDE_DIRECTORY "FreeImage.h" 
	HINTS "/usr/local/include" "/usr/include" $ENV{FREEIMAGE_INCLUDEDIR} )

if( NOT FREEIMAGE_INCLUDE_DIRECTORY )
	message( FATAL_ERROR "FreeImage not found." )
endif() 

include_directories( ${FREEIMAGE_INCLUDE_DIRECTORY} )

if( NOT SNAKEOIL_EXTERNAL_LINKAGE_REQUIRED ) 
return()
endif()

find_library( FREEIMAGE_LIBRARY_DEBUG NAMES "FreeImaged" "FreeImage" 
	HINTS "/usr/lib64/" "/usr/local/lib64/" $ENV{FREEIMAGE_LIBRARYDIR} )
	
find_library( FREEIMAGE_LIBRARY_RELEASE "FreeImage" 
	HINTS "/usr/lib64/" "/usr/local/lib64/" $ENV{FREEIMAGE_LIBRARYDIR} )

set( FREEIMAGE_LIBRARIES 
	debug ${FREEIMAGE_LIBRARY_DEBUG}
	optimized ${FREEIMAGE_LIBRARY_RELEASE} 
)


