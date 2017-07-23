//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_MODULE_IMIDI_MODULE_H_
#define _SNAKEOIL_DEVICE_MODULE_IMIDI_MODULE_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../result.h"
#include "../api.h"

#include "../devices/midi/midi_message.h"

namespace so_device
{
    class SNAKEOIL_DEVICE_API imidi_module
    {

    public:

        virtual so_device::result register_device( so_device::key_cref_t, 
            so_device::midi_device_ptr_t ) = 0 ;
        virtual so_device::result unregister_device( so_device::key_cref_t ) = 0 ;

        virtual so_device::midi_device_ptr_t find_device( so_device::key_cref_t ) = 0 ;

    public:

        virtual void_t update( void_t ) = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
