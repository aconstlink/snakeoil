


set( RTMIDI_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )
set( BUILD_STATIC OFF  )
else()
set( BUILD_STATIC ON )
endif()
option( BUILD_SHARED_LIBS "" ${SNAKEOIL_LIBRARY_BUILD_SHARED} )
option( BUILD_STATIC_LIBS "" ${BUILD_STATIC} )
option( BUILD_TESTING "" OFF )

add_subdirectory( rtmidi-3.0.0 )


set( RTMIDI_VERSION 3.0.0 )

if( TARGET rtmidi )
set_target_properties( rtmidi PROPERTIES FOLDER "snakeoil/externals/rtmidi" )
else()
set_target_properties( rtmidi_static PROPERTIES FOLDER "snakeoil/externals/rtmidi" )
endif()
set_target_properties( uninstall PROPERTIES FOLDER "snakeoil/externals/rtmidi" )
set( RTMIDI_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/rtmidi-3.0.0" )

if( TARGET rtmidi )
target_compile_definitions( rtmidi PRIVATE RTMIDI_DO_NOT_ENSURE_UNIQUE_PORTNAMES )
else()
target_compile_definitions( rtmidi_static PRIVATE RTMIDI_DO_NOT_ENSURE_UNIQUE_PORTNAMES )
endif()

if( SNAKEOIL_COMPILER_MSC )
  if( TARGET rtmidi )
    target_compile_options( rtmidi PRIVATE /wd4267 /wd4996 )
  endif()
  if( TARGET rtmidi_static )
    target_compile_options( rtmidi_static PRIVATE /wd4267 /wd4996 )
  endif()
endif()

if( TARGET rtmidi )
target_include_directories( rtmidi PUBLIC $<BUILD_INTERFACE:${RTMIDI_INCLUDE_DIRS}> )
else()
target_include_directories( rtmidi_static PUBLIC $<BUILD_INTERFACE:${RTMIDI_INCLUDE_DIRS}> )
endif() 

add_library( rtmidi_trg INTERFACE )
if( SNAKEOIL_LIBRARY_BUILD_SHARED )
  target_link_libraries( rtmidi_trg INTERFACE rtmidi )
else()
  target_link_libraries( rtmidi_trg INTERFACE rtmidi_static )
endif()

if( TARGET rtmidi )
export( TARGETS rtmidi NAMESPACE snakeoil:: APPEND FILE ${SNAKEOIL_BINARY_DIR}/snakeoil-targets.cmake )
endif()

unset( JACK_LIB CACHE )
unset( PKG_CONFIG_EXECUTABLE CACHE )
unset( BUILD_SHARED_LIBS CACHE )
unset( BUILD_STATIC_LIBS CACHE )
unset( BUILD_TESTING CACHE )
unset( RTMIDI_API_CORE CACHE )
unset( RTMIDI_API_JACK CACHE )
unset( RTMIDI_API_WINMM CACHE )
