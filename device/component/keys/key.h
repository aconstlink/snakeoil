//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_COMPONENTS_KEY_H_
#define _SNAKEOIL_DEVICE_COMPONENTS_KEY_H_

#if 0

#include "comp.h"

namespace so_device
{
    namespace so_component
    {
        enum class key_state
        {
            undefined,
            press,
            release
        };

        struct key : public comp<key>
        {
            static comp_type const ct ;

            size_t cid = size_t(-1) ;
            key_state state = key_state::undefined ;
            float_t intensity = 0.0f ;
        };
        comp_type const key::ct = comp_type::key ;
    }
}
#endif

#endif
