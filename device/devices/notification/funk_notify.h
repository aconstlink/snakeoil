//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_DEVICES_NOTIFICATION_FUNK_NOTIFY_H_
#define _SNAKEOIL_DEVICE_DEVICES_NOTIFICATION_FUNK_NOTIFY_H_

#include "iuser_notify.h"

namespace so_device
{
    class SNAKEOIL_DEVICE_API funk_notify : public iuser_notify
    {
        so_this_typedefs( funk_notify ) ;

    public:

        typedef std::function< void_t( so_device::idevice_ptr_t, so_std::string_cref_t name,
            so_device::so_input::iinput_component_ptr_t comp_ptr ) > user_funk_t ;
    
    private:

        user_funk_t _funk ;

    public:

        funk_notify( user_funk_t funk ) ;

        static this_ptr_t create( user_funk_t funk, so_memory::purpose_cref_t p ) ;
        static this_ptr_t create( this_rref_t rhv, so_memory::purpose_cref_t p ) ;

        static void_t destroy( this_ptr_t ptr ) ;


    public:

        virtual void_t on_change( so_device::idevice_ptr_t, so_std::string_cref_t name,
            so_device::so_input::iinput_component_ptr_t comp_ptr ) ;

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( funk_notify ) ;
}

#endif
