#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

include_directories( 
    ${XINPUT_INCLUDE_DIRS}
)

#
# components
#

set( COMPONENT_SOURCES
    component/iinput_component.h
    component/ioutput_component.h
    component/input_component.h
    component/output_component.h

    component/buttons/button_state.h
    component/buttons/binary_button.h
    component/buttons/value_button.h

    component/keys/key.h
    component/points/point.h

    component/leds/led_state.h
    component/leds/binary_led.h

    component/motors/motor_state.h
    component/motors/value_motor.h

    component/scrolls/scroll.h
    component/sliders/slider_state.h
    component/sliders/slider.h
    component/knobs/rotary_knob.h

    component/sticks/stick_state.h
    component/sticks/stick.h
    component/sticks/value_stick.h
)

#
# devices
#

set( MIDI_SOURCES 
    devices/midi/midi_message.h
    devices/midi/midi_device.h
    devices/midi/midi_device.cpp
    devices/midi/midi_enums.h
    devices/midi/akai/akai.h
    devices/midi/akai/akai.cpp
    devices/midi/akai/midimix.cpp
    devices/midi/korg/korg.h
    devices/midi/korg/korg.cpp
    devices/midi/korg/nanokontrol2.cpp
    devices/midi/korg/nanopad2.cpp
    devices/midi/behringer/cmd_mm_1.h
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
    ${MIDI_SOURCES}
    ${VDEV_SOURCES}
)


#
# modules
#
set( MODULE_SOURCES
    modules/imidi_module.h
    modules/igamepad_module.h
)
set( WIN32_MODULE_SOURCES
    modules/win32/midi/midi_module.h
    modules/win32/midi/midi_module.cpp
    modules/win32/xinput/xinput_module.h
    modules/win32/xinput/xinput_module.cpp
    modules/win32/xinput/xinput_device.h
    modules/win32/xinput/xinput_device.cpp
    modules/win32/xinput/xinput_enums.h
    modules/win32/xinput/xinput_mappings.h
    modules/win32/xinput/xinput_mappings.cpp
)

set( WIN32_SOURCES

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
        ${WIN32_MODULE_SOURCES}
        ${WIN32_SYSTEM_SOURCES}
    )	
    set( OS_SPECIFIC_LIBS winmm.lib )
    
endif()

#
# sources
#
set( SOURCES 
    api.h
    result.h
    typedefs.h
    protos.h
    ${COMPONENT_SOURCES}
    ${DEVICE_SOURCES}
    ${SYSTEM_SOURCES}
    ${MODULE_SOURCES}
    ${OS_SPECIFIC_SOURCES}
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
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )

