//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_PROTOS_H_
#define _SNAKEOIL_DEVICE_PROTOS_H_

#include <snakeoil/core/macros/typedef.h>

namespace so_device
{
    namespace so_input
    {
        so_struct_proto_typedefs( iinput_component ) ;
    }
    namespace so_output
    {
        so_struct_proto_typedefs( ioutput_component ) ;
    }

    namespace so_vdev
    {
        so_class_proto_typedefs( ivdev ) ;
    }

    namespace so_vgamepad
    {
        
        so_class_proto_typedefs( vgamepad ) ;
        so_class_proto_typedefs( xbox_360 ) ;
    }
    
    so_class_proto_typedefs( iapi ) ;
    so_class_proto_typedefs( imidi_api ) ;
    so_class_proto_typedefs( igamepad_api ) ;
    so_class_proto_typedefs( imouse_api ) ;
    so_class_proto_typedefs( ikeyboard_api ) ;

    so_class_proto_typedefs( idevice ) ;
    so_class_proto_typedefs( midi_device ) ;
    so_class_proto_typedefs( gamepad_device ) ;
    so_class_proto_typedefs( keyboard_device ) ;
    so_class_proto_typedefs( ascii_keyboard ) ;
    so_class_proto_typedefs( mouse_device ) ;
    so_class_proto_typedefs( three_button_mouse ) ;


    so_class_proto_typedefs( idevice_system ) ;
    so_class_proto_typedefs( device_system ) ;

    so_class_proto_typedefs( iuser_notify ) ;

    so_class_proto_typedefs( module_registry ) ;
    so_class_proto_typedefs( imodule ) ;
    so_class_proto_typedefs( imidi_module ) ;
    so_class_proto_typedefs( igamepad_module ) ;
    so_class_proto_typedefs( imouse_module ) ;
    so_class_proto_typedefs( ikeyboard_module ) ;

    so_class_proto_typedefs( imidi_notify ) ;

    namespace so_win32
    {
        so_class_proto_typedefs( rawinput_api ) ;
    }
}

#endif

