//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_API_IMIDI_API_H_
#define _SNAKEOIL_DEVICE_API_IMIDI_API_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../result.h"
#include "../api.h"

#include "../devices/midi/midi_message.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_device
{
    class SNAKEOIL_DEVICE_API imidi_api
    {

    public:

        /// create all pluged-in devices that are registered
        virtual void_t create_devices( so_device::imidi_module_ptr_t ) = 0 ;


        virtual void_t get_device_names( so_std::vector< so_std::string_t > & ) const = 0 ;
        virtual so_device::midi_device_ptr_t find_midi_device( so_device::key_cref_t ) = 0 ;
        virtual so_device::midi_device_ptr_t find_any_midi_device( void_t ) = 0 ;

        virtual void_t install_midi_notify( so_device::imidi_notify_ptr_t ) = 0 ;

    public:

        virtual void_t update( void_t ) = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
