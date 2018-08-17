#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# components
#

set( COMPONENT_SOURCES
  component/input_component_type.h
  component/output_component_type.h
  component/iinput_component.h
  component/ioutput_component.h
  component/input_component.h
  component/output_component.h

  component/buttons/button_id.h
  component/buttons/button_state.h
  component/buttons/binary_button.h
  component/buttons/value_button.h
  component/buttons/three_button.h
  component/buttons/three_button_mapping.h

  component/keys/binary_key.h
  component/keys/key_state.h
  component/keys/key_id.h
  component/keys/ascii_key.h
  component/keys/ascii_key_mapping.h

  component/points/point.h
  
  component/leds/led_state.h
  component/leds/binary_led.h
  component/leds/multi_led.h

  component/motors/motor_state.h
  component/motors/value_motor.h

  component/scrolls/scroll.h
  component/sliders/slider_state.h
  component/sliders/slider.h
  component/knobs/rotary_knob.h

  component/sticks/stick_state.h
  component/sticks/stick.h
  component/sticks/value_stick.h

  component/touch/single_touch.h
  component/touch/touch_state.h

  component/lights/light_state.h
  component/lights/light_bar.h
)

#
# devices
#

set( MIDI_SOURCES 
    devices/midi/midi_message.h
    devices/midi/midi_device.h
    devices/midi/midi_device.cpp
    devices/midi/midi_enums.h
    devices/midi/midi_device_desc.h
)

set( KEYBOARD_SOURCES 
  devices/keyboard/ikeyboard_device.h
  devices/keyboard/ascii_keyboard.h
  devices/keyboard/ascii_keyboard.cpp
)

set( MOUSE_SOURCES 
  devices/mouse/imouse_device.h
  devices/mouse/three_button_mouse.h
  devices/mouse/three_button_mouse.cpp  
)

#
# virtual devices
#
set( VDEV_SOURCES 
    vdev/ivdev.h
    vdev/gamepad/xbox/xbox_enums.h
    vdev/gamepad/xbox/xbox_360.h
    vdev/gamepad/xbox/xbox_360.cpp
)

set( DEVICE_SOURCES
    devices/idevice.h
    devices/gamepad/gamepad_message.h
    devices/gamepad/gamepad_device.h
    devices/gamepad/gamepad_device.cpp
    devices/gamepad/gamepad_enums.h
    devices/notification/iuser_notify.h
    devices/notification/funk_notify.h
    devices/notification/funk_notify.cpp
    ${MOUSE_SOURCES}
    ${KEYBOARD_SOURCES}
    ${MIDI_SOURCES}
    ${VDEV_SOURCES}
)

#
# modules
#
set( MODULES_SOURCES
  modules/module_structs.h
  modules/script_type.h
  modules/module_registry.h
  modules/module_registry.cpp
  modules/imodule.h
  modules/igamepad_module.h
  modules/ikeyboard_module.h
  modules/imouse_module.h
  modules/imidi_module.h
  modules/system/system_module.h
  modules/system/system_module.cpp
  modules/system/create_korg_nanopad2.cpp
  modules/system/create_korg_nanokontrol2.cpp
  modules/system/create_akai_apckeys25.cpp
  modules/system/create_akai_midimix.cpp
  modules/system/create_behringer_cmdmm1.cpp
)

#
# apis
#
set( API_SOURCES
  api/iapi.h
  api/midi_notify.h
  api/gamepad_notify.h
  api/imidi_api.h
  api/igamepad_api.h
  api/imouse_api.h
  api/ikeyboard_api.h    
)

set( WIN32_API_SOURCES
    api/win32/midi/win32_midi_api.h
    api/win32/midi/win32_midi_api.cpp
    api/win32/xinput/xinput_api.h
    api/win32/xinput/xinput_api.cpp
    api/win32/xinput/xinput_device.h
    api/win32/xinput/xinput_device.cpp
    api/win32/xinput/xinput_enums.h
    api/win32/xinput/xinput_mappings.h
    api/win32/xinput/xinput_mappings.cpp
    api/win32/rawinput/rawinput_api.h
    api/win32/rawinput/rawinput_api.cpp
    api/win32/rawinput/rawinput_button_mapping.h
    api/win32/map_virtual_key_to_ascii_key.h
)

set( WIN32_SYSTEM_SOURCES
    system/win32/win32_device_system.cpp
)

set( SYSTEM_SOURCES
    system/idevice_system.h
    system/device_system.h
    system/device_system.cpp
)

set( OS_SPECIFIC_SOURCES )

if( SNAKEOIL_TARGET_OS_WIN )

    if( SNAKEOIL_COMPILER_MSC )
        # warning C4005 : '__useHeader' : macro redefinition 
        # Note: C4005 seem to be a redefinition bug in the window header files.
        set( SNAKEOIL_UNWANTED_COMPILER_OPTIONS "/wd4005" )
        set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SNAKEOIL_UNWANTED_COMPILER_OPTIONS}" )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SNAKEOIL_UNWANTED_COMPILER_OPTIONS}" )
    endif()

    set( OS_SPECIFIC_SOURCES
        ${OS_SPECIFIC_SOURCES}
        ${WIN32_SOURCES}
        ${WIN32_API_SOURCES}
        ${WIN32_SYSTEM_SOURCES}
    )	
    set( OS_SPECIFIC_LIBS winmm.lib ${WDDK_LIBRARIES} )

    # keep WDDK include up front because on Win7 the
    # the SDK needs to appear in front of the xinput stuff
    include_directories( 
      ${WDDK_INCLUDE_DIRS}
      ${XINPUT_INCLUDE_DIRS}
    )
    
endif()

#
# sources
#
set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h
    global.h
    global.cpp
    ${COMPONENT_SOURCES}
    ${DEVICE_SOURCES}
    ${SYSTEM_SOURCES}
    ${API_SOURCES}
    ${OS_SPECIFIC_SOURCES}
    ${MODULES_SOURCES}
)

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

    add_definitions( 
        -DSNAKEOIL_DEVICE_EXPORT 
        -DSNAKEOIL_LOG_IMPORT
        -DSNAKEOIL_MEMORY_IMPORT
        )
endif()

# snakeoil
target_link_libraries( ${CUR_LIB_NAME}
    snakeoil_log
    snakeoil_memory
)

#all 3rd party
if( ${SNAKEOIL_TARGET_OS_WIN} )
    target_link_libraries( ${CUR_LIB_NAME}
        ${XINPUT_LIBRARIES}
        ${OS_SPECIFIC_LIBS}
    )
endif()
    
###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
install_library( ${CUR_LIB_NAME} )

