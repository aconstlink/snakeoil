//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_API_WIN32_XINPUT_API_H_
#define _SNAKEOIL_DEVICE_API_WIN32_XINPUT_API_H_

#include "../../igamepad_api.h"
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
        class xinput_api : public igamepad_api
        {
            so_this_typedefs( xinput_api ) ;

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

            so_typedefs( so_std::vector<igamepad_notify_ptr_t>, notifies ) ;
            
        private:

            devices_t _devices ;
            xinput_mappings _xinput_mappings ;

            notifies_t _notifies ;

        public:

            xinput_api( void_t ) ;
            xinput_api( this_rref_t ) ;
            virtual ~xinput_api( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual void_t create_devices( so_device::igamepad_module_ptr_t ) ;

            virtual so_device::result register_device( so_device::key_cref_t,
                so_device::gamepad_device_ptr_t ) ;
            virtual so_device::result unregister_device( so_device::key_cref_t ) ;

        public:

            virtual void_t update_gamepad( void_t ) ;
            virtual void_t destroy( void_t ) ;

            virtual so_device::gamepad_device_ptr_t find_any_device( void_t ) ;
            virtual so_device::so_vgamepad::xbox_360_ptr_t find_device( size_t const ) ;
            virtual bool_t register_for_any_device( so_device::so_vdev::ivdev_ptr_t ) ;
            virtual bool_t register_device( size_t const, so_device::so_vdev::ivdev_ptr_t ) ;
            virtual bool_t unregister_virtual_device( so_device::so_vdev::ivdev_ptr_t ) ;

            virtual void_t install_gamepad_notify( so_device::igamepad_notify_ptr_t ) ;

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
        so_typedef( xinput_api ) ;
    }
}

#endif
