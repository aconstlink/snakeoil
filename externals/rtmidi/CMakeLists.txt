


set( RTMIDI_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
add_subdirectory( rtmidi-3.0.0 )
set( RTMIDI_VERSION 3.0.0 )
set_target_properties( rtmidi PROPERTIES FOLDER "snakeoil/externals/rtmidi" )
set_target_properties( rtmidi_static PROPERTIES FOLDER "snakeoil/externals/rtmidi" )
set_target_properties( uninstall PROPERTIES FOLDER "snakeoil/externals/rtmidi" )
set( RTMIDI_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/rtmidi-3.0.0" )

target_compile_definitions( rtmidi PRIVATE RTMIDI_DO_NOT_ENSURE_UNIQUE_PORTNAMES )
target_compile_definitions( rtmidi_static PRIVATE RTMIDI_DO_NOT_ENSURE_UNIQUE_PORTNAMES )

if( SNAKEOIL_COMPILER_MSC )
  target_compile_options( rtmidi PRIVATE /wd4267 /wd4996 )
  target_compile_options( rtmidi_static PRIVATE /wd4267 /wd4996 )
endif()

target_include_directories( rtmidi PUBLIC $<BUILD_INTERFACE:${RTMIDI_INCLUDE_DIRS}> )
target_include_directories( rtmidi_static PUBLIC $<BUILD_INTERFACE:${RTMIDI_INCLUDE_DIRS}> )

export( TARGETS rtmidi NAMESPACE snakeoil:: APPEND FILE ${SNAKEOIL_BINARY_DIR}/snakeoil-targets.cmake )
