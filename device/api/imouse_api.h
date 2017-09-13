//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_API_IMOUSE_API_H_
#define _SNAKEOIL_DEVICE_API_IMOUSE_API_H_

#include "iapi.h"

namespace so_device
{
    class SNAKEOIL_DEVICE_API imouse_api : public so_device::iapi
    {

    public:

        virtual so_device::three_button_mouse_ptr_t find_three_button_mouse( void_t ) = 0 ;
        virtual void_t update_mouse( void_t ) = 0 ;
    };
}

#endif
