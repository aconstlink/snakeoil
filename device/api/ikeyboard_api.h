//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_API_IKEYBOARD_API_H_
#define _SNAKEOIL_DEVICE_API_IKEYBOARD_API_H_

#include "iapi.h"

namespace so_device
{
    class SNAKEOIL_DEVICE_API ikeyboard_api : public so_device::iapi
    {

    public:

        virtual so_device::ascii_keyboard_ptr_t find_ascii_keyboard( void_t ) = 0 ;
        virtual void_t update_keyboard( void_t ) = 0 ;
    };
}

#endif
