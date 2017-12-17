//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_API_WIN32_RAW_RAW_API_H_
#define _SNAKEOIL_DEVICE_API_WIN32_RAW_RAW_API_H_

#include "../../imouse_api.h"
#include "../../ikeyboard_api.h"
#include "../../igamepad_api.h"

#include "../../../component/buttons/button_state.h"
#include "../../../component/buttons/three_button.h"
#include "../../../component/keys/ascii_key.h"
#include "../../../component/keys/key_state.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/std/container/map.hpp>
#include <snakeoil/math/vector/vector2.hpp>

#include <windows.h>

namespace so_device
{
    namespace so_win32
    {
        class SNAKEOIL_DEVICE_API rawinput_api : public so_device::imouse_api, public so_device::ikeyboard_api, public so_device::igamepad_api
        {
            so_this_typedefs( rawinput_api ) ; 

        private: // mouse typedefs

            typedef so_device::three_button three_button_t ;
            typedef so_device::button_state button_state_t ;

            typedef std::pair< three_button_t, button_state_t> mouse_button_item_t ;
            typedef so_std::vector< mouse_button_item_t > mouse_button_items_t ;

            typedef so_std::vector< so_math::vec2f_t > pointer_coords_t ;

        private: // keyboard typedefs

            typedef so_device::ascii_key ascii_key_t ;
            typedef so_device::key_state key_state_t ;

            typedef std::pair< ascii_key_t, key_state_t > ascii_keyboard_key_item_t ;
            typedef so_std::vector< ascii_keyboard_key_item_t > ascii_keyboard_keys_t ;
            
            so_typedefs( so_std::vector< ascii_keyboard_ptr_t >, ascii_keyboards ) ;
            so_typedefs( so_std::vector< three_button_mouse_ptr_t >, three_button_mice ) ;

        private: // gamepad typedefs

            so_typedefs( so_std::vector< igamepad_notify_ptr_t >, gamepad_notifies ) ;

        private:

            mouse_button_items_t _three_button_items ;
            pointer_coords_t _pointer_coords_global ;
            pointer_coords_t _pointer_coords_local ;

            ascii_keyboard_keys_t _ascii_keyboard_keys ;

            ascii_keyboards_t _ascii_keyboards ;
            three_button_mice_t _three_button_mice ;

            gamepad_notifies_t _gamepad_notifies ;
            so_std::map< so_std::string_t, uint_t > _gamepad_ids ;

        private:

            so_thread::mutex_t _buffer_mtx ;

            UINT _raw_input_data_size = 0u ;
            LPBYTE _raw_input_buffer = nullptr ;

        public:

            rawinput_api( void_t ) ;
            rawinput_api( this_cref_t ) = delete ;
            rawinput_api( this_rref_t ) ;
            virtual ~rawinput_api( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_device::ascii_keyboard_ptr_t find_ascii_keyboard( void_t ) ;
            virtual so_device::three_button_mouse_ptr_t find_three_button_mouse( void_t ) ;

            bool_t handle_input_event( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) ;

        public:
            
            virtual void_t create_devices( so_device::igamepad_module_ptr_t ) ;
            virtual so_device::result register_device( so_device::key_cref_t,
                so_device::gamepad_device_ptr_t ) ;
            virtual so_device::result unregister_device( so_device::key_cref_t ) ;
            virtual so_device::gamepad_device_ptr_t find_any_device( void_t )  ;
            virtual so_device::so_vgamepad::xbox_360_ptr_t find_device( size_t const ) ;
            virtual bool_t register_for_any_device( so_device::so_vdev::ivdev_ptr_t )  ;
            virtual bool_t register_device( size_t const, so_device::so_vdev::ivdev_ptr_t )  ;
            virtual bool_t unregister_virtual_device( so_device::so_vdev::ivdev_ptr_t ) ;

            virtual void_t install_gamepad_notify( so_device::igamepad_notify_ptr_t )  ;

        public:

            virtual void_t update_mouse( void_t ) ; 
            virtual void_t update_keyboard( void_t ) ;
            virtual void_t update_gamepad( void_t ) ;

            virtual void_t destroy( void_t ) ;
        };
    }
}

#endif
