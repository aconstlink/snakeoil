//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_MODULES_WIN32_XINPUT_MODULE_H_
#define _SNAKEOIL_DEVICE_MODULES_WIN32_XINPUT_MODULE_H_

#include "../../igamepad_module.h"
#include "../../../devices/gamepad/gamepad_device.h"
#include "../../../devices/gamepad/gamepad_message.h"

#include "xinput_device.h"
#include "xinput_mappings.h"
#include "xinput_enums.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_device
{
    namespace so_win32
    {
        class xinput_module : public igamepad_module
        {
            so_this_typedefs( xinput_module ) ;

            so_typedefs( so_std::vector<so_device::so_vdev::ivdev_ptr_t>, vdevs ) ;

            struct gamepad_data
            {
                so_device::so_win32::xinput_device_ptr_t xinput_ptr = nullptr ;
                so_device::gamepad_device_ptr_t device_ptr = nullptr ;
                vdevs_t vdevs ;
            };
            so_typedef( gamepad_data ) ;
            so_typedefs( so_std::vector<gamepad_data_t>, devices ) ;

            enum class which_stick
            {
                left,
                right
            };
            
            enum class which_trigger
            {
                left,
                right
            };
            
        private:

            devices_t _devices ;
            xinput_mappings _xinput_mappings ;

        public:

            xinput_module( void_t ) ;
            xinput_module( this_rref_t ) ;
            virtual ~xinput_module( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_device::result register_device( so_device::key_cref_t,
                so_device::gamepad_device_ptr_t ) ;
            virtual so_device::result unregister_device( so_device::key_cref_t ) ;

        public:

            virtual void_t update( void_t ) ;
            virtual void_t destroy( void_t ) ;

            virtual so_device::gamepad_device_ptr_t find_any_device( void_t ) ;
            virtual bool_t register_for_any_device( so_device::so_vdev::ivdev_ptr_t ) ;
            virtual bool_t unregister_virtual_device( so_device::so_vdev::ivdev_ptr_t ) ;

        private:

            void_t init_gamepads( void_t ) ;
            so_device::gamepad_device_ptr_t create_gamepad( void_t ) ;

        private:

           
            bool_t handle_button( XINPUT_STATE const & state, DWORD const button_flag,
                so_device::so_win32::xinput_device_cref_t xdev,
                so_device::gamepad_device_ref_t ) ;

            bool_t handle_stick( this_t::which_stick, XINPUT_STATE const & state,
                so_device::so_win32::xinput_device_cref_t xdev,
                so_device::gamepad_device_ref_t ) ;

            bool_t handle_trigger( this_t::which_trigger, XINPUT_STATE const & state,
                so_device::so_win32::xinput_device_cref_t xdev,
                so_device::gamepad_device_ref_t ) ;
        };
        so_typedef( xinput_module ) ;
    }
}

#endif
