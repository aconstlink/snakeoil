#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( FMOD_FOUND False )

option( SNAKEOIL_FMOD_ENABLE "Enable Fmod Sound System" OFF )
if( NOT SNAKEOIL_FMOD_ENABLE )
  return()
endif()
message( STATUS "[Enabled] : FMOD SDK" )

find_package( fmod CONFIG REQUIRED )
