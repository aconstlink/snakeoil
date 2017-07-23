#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( FFMPEG_BASE_DIR ${_FFMPEG_BASE_DIR_} )
#set( FFMPEG_BASE_DIR E:/libraries/ffmpeg-3.2.4 )

# libavcodec

add_library(avcodec SHARED IMPORTED)

set_property(TARGET avcodec APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(avcodec PROPERTIES
    IMPORTED_IMPLIB_DEBUG "${FFMPEG_BASE_DIR}/lib64/avcodec.lib"
    IMPORTED_LOCATION_DEBUG "${FFMPEG_BASE_DIR}/lib/libavcodec.a"
  )

set_property(TARGET avcodec APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(avcodec PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${FFMPEG_BASE_DIR}/lib64/avcodec.lib"
  IMPORTED_LOCATION_RELEASE "${FFMPEG_BASE_DIR}/lib/libavcodec.a"
  )
  
#avdevice
add_library(avdevice SHARED IMPORTED)

set_property(TARGET avdevice APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(avdevice PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${FFMPEG_BASE_DIR}/lib64/avdevice.lib"
  IMPORTED_LOCATION_DEBUG "${FFMPEG_BASE_DIR}/lib/libavdevice.a"
  )

set_property(TARGET avdevice APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(avdevice PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${FFMPEG_BASE_DIR}/lib64/avdevice.lib"
  IMPORTED_LOCATION_RELEASE "${FFMPEG_BASE_DIR}/lib/libavdevice.a"
  )
  
#libavfilter
add_library(avfilter SHARED IMPORTED)

set_property(TARGET avfilter APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(avfilter PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${FFMPEG_BASE_DIR}/lib64/avfilter.lib"
  IMPORTED_LOCATION_DEBUG "${FFMPEG_BASE_DIR}/lib/libavfilter.a"
  )

set_property(TARGET avfilter APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(avfilter PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${FFMPEG_BASE_DIR}/lib64/avfilter.lib"
  IMPORTED_LOCATION_RELEASE "${FFMPEG_BASE_DIR}/lib/libavfilter.a"
  )

#libavformat
add_library(avformat SHARED IMPORTED)

set_property(TARGET avformat APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(avformat PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${FFMPEG_BASE_DIR}/lib64/avformat.lib"
  IMPORTED_LOCATION_DEBUG "${FFMPEG_BASE_DIR}/lib/libavformat.a"
  )

set_property(TARGET avformat APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(avformat PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${FFMPEG_BASE_DIR}/lib64/avformat.lib"
  IMPORTED_LOCATION_RELEASE "${FFMPEG_BASE_DIR}/lib/libavformat.a"
  )
  
#libavutil
add_library(avutil SHARED IMPORTED)

set_property(TARGET avutil APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(avutil PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${FFMPEG_BASE_DIR}/lib64/avutil.lib"
  IMPORTED_LOCATION_DEBUG "${FFMPEG_BASE_DIR}/lib/libavutil.a"
  )

set_property(TARGET avutil APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(avutil PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${FFMPEG_BASE_DIR}/lib64/avutil.lib"
  IMPORTED_LOCATION_RELEASE "${FFMPEG_BASE_DIR}/lib/libavutil.a"
  )
  
#libpostproc
#libswresample
add_library(swresample SHARED IMPORTED)

set_property(TARGET swresample APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(swresample PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${FFMPEG_BASE_DIR}/lib64/swresample.lib"
  IMPORTED_LOCATION_DEBUG "${FFMPEG_BASE_DIR}/lib/libswresample.a"
  )

set_property(TARGET swresample APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(swresample PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${FFMPEG_BASE_DIR}/lib64/swresample.lib"
  IMPORTED_LOCATION_RELEASE "${FFMPEG_BASE_DIR}/lib/libswresample.a"
  )
  
#libswscale
add_library(swscale SHARED IMPORTED)

set_property(TARGET swscale APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(swscale PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${FFMPEG_BASE_DIR}/lib64/swscale.lib"
  IMPORTED_LOCATION_DEBUG "${FFMPEG_BASE_DIR}/lib/libswscale.a"
  )

set_property(TARGET swscale APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(swscale PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${FFMPEG_BASE_DIR}/lib64/swscale.lib"
  IMPORTED_LOCATION_RELEASE "${FFMPEG_BASE_DIR}/lib/libswscale.a"
  )
  
unset( FFMPEG_BASE_DIR )