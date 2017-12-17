#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( FMOD_FOUND False )

#
# SECTION : Option
#
option( FMOD_ENABLE "Enable Fmod Sound System" ${FMOD_ENABLE} )

if( FMOD_ENABLE )
	message( STATUS "[Enabled] : FMOD SDK" )
else()
	message( STATUS "[Disabled] : FMOD SDK" )
	return()
endif()

set( FBXSDK_DEFINITIONS 
	-DFBXSDK_ENABLE
	)

find_package( fmod CONFIG REQUIRED )
	
	


