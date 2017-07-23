#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

####################################################################
# Import targets
####################################################################
get_filename_component( CMAKE_CURRENT_LIST_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set( _FFMPEG_BASE_DIR_ ${CMAKE_CURRENT_LIST_DIR} )
include( "${_FFMPEG_BASE_DIR_}/ffmpeg-targets.cmake" )
message( ${CMAKE_CURRENT_LIST_DIR} )

set( FFMPEG_LIBRARIES
    avformat
    avcodec
    avutil
    swresample
    swscale
   )

####################################################################
# Definitions
####################################################################
#set( FFMPEG_DEFINITIONS @ALL_DEFINITIONS@ )

####################################################################
# Directories
####################################################################
set( FFMPEG_INCLUDE_DIRS "${_FFMPEG_BASE_DIR_}/include" )


