//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_MODULES_SYSTEM_SYSTEM_MODULE_H_
#define _SNAKEOIL_DEVICE_MODULES_SYSTEM_SYSTEM_MODULE_H_

#include "../imodule.h"
#include "../imidi_module.h"

namespace so_device
{
    class SNAKEOIL_DEVICE_API system_module : public so_device::imidi_module
    {
        so_this_typedefs( system_module ) ;

    public:

        system_module( void_t ) ;
        system_module( this_cref_t ) = delete ;
        system_module( this_rref_t ) ;
        virtual ~system_module( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual so_device::midi_device_ptr_t create_midi_device( so_std::string_cref_t ) ;

    public:

        virtual void_t destroy( void_t ) ;

    private:

        so_device::midi_device_ptr_t create_korg_nanopad2( void_t ) ;
        so_device::midi_device_ptr_t create_korg_nanokontrol2( void_t ) ;
        so_device::midi_device_ptr_t create_akai_midimix( void_t ) ;
        so_device::midi_device_ptr_t create_akai_apckey25( void_t ) ;
        so_device::midi_device_ptr_t create_behringer_cmdmm1( void_t ) ;
        so_device::midi_device_ptr_t create_empty_midi_device( so_std::string_cref_t ) ;

    };
    so_typedef( system_module ) ;
}

#endif