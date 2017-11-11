#pragma once

#include <snakeoil/device/protos.h>
#include <snakeoil/math/vector/vector2.hpp>

namespace so_ui
{
    enum class mouse_event_type
    {
        enter,
        leave,
        inside
    };

    struct mouse_event
    {
        mouse_event_type me_type ;
        so_math::vec2f_t local_coords ;
        so_device::three_button_mouse_ptr_t mptr ;
    };
    so_typedef( mouse_event ) ;
}