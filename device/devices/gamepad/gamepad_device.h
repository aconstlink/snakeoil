//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_DEVICES_GAMEPAD_GAMEPAD_H_
#define _SNAKEOIL_DEVICE_DEVICES_GAMEPAD_GAMEPAD_H_

#include "../idevice.h"

#include "gamepad_message.h"
#include "gamepad_enums.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/std/string/string.hpp>

#include <functional>

namespace so_device
{
    class SNAKEOIL_DEVICE_API gamepad_device : public idevice
    {
        so_this_typedefs( gamepad_device ) ;

    public: // input components typedefs

        typedef so_device::so_component::iinput_component_ptr_t in_comp_ptr_t ;
        typedef std::function< bool_t( in_comp_ptr_t, gamepad_message_cref_t ) > in_message_handle_funk_t ;
        typedef std::function< bool_t( in_comp_ptr_t ) > follow_up_funk_t ;
        typedef std::function< void_t( so_std::string_cref_t, in_comp_ptr_t ) > check_state_funk_t ;

        struct input_component_logic
        {
            in_message_handle_funk_t handle_funk ;
            follow_up_funk_t follow_up_funk ;
        };
        so_typedef( input_component_logic ) ;

    public: // output components typedefs

        typedef so_device::so_component::ioutput_component_ptr_t out_comp_ptr_t ;
        typedef std::function< gamepad_output_result( out_comp_ptr_t, gamepad_message_ref_t ) > out_message_handle_funk_t ;
        
        struct output_component_logic
        {
            out_message_handle_funk_t handle_funk ;
        };
        so_typedef( output_component_logic ) ;

        typedef std::function< bool_t( so_device::so_component::ioutput_component_ptr_t ) >
            trigger_funk_t ;

        struct trigger_data
        {
            so_device::key_t key ;
            trigger_funk_t funk ;
        };
        so_typedef( trigger_data ) ;
        so_typedefs( so_std::vector<trigger_data_t>, trigger_datas ) ;

    private:

        struct store_in_data
        {
            so_std::string_t name ;
            in_comp_ptr_t cptr ;
            input_component_logic logic ;
        };
        so_typedef( store_in_data ) ;
        so_typedefs( so_std::vector<store_in_data_t>, in_datas ) ;

        struct store_out_data
        {
            so_std::string_t name ;
            out_comp_ptr_t cptr ;

            output_component_logic_t logic ;
        };
        so_typedef( store_out_data ) ;
        so_typedefs( so_std::vector<store_out_data_t>, out_datas ) ;

    public:

        so_typedefs( so_std::vector<gamepad_message_t>, gamepad_messages ) ;
        so_typedefs( so_std::vector<iuser_notify_ptr_t>, notifys ) ;

    private: // input components

        in_datas_t _in_datas ;
        in_datas_t _changed_comps ;

        so_thread::mutex_t _mtx_in ;
        gamepad_messages_t _in_msgs ;

    private: // output components

        out_datas_t _out_datas ;
        out_datas_t _triggered_component ;

        so_thread::mutex_t _mtx_out ;
        gamepad_messages_t _out_msgs ;


        so_thread::mutex_t _trigger_data_mtx ;
        trigger_datas_t _trigger_datas ;

    private:

        notifys_t _notifys ;

    public:

        gamepad_device( void_t ) ;
        gamepad_device( this_rref_t ) ;
        ~gamepad_device( void_t ) ;

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_device::result receive_message( so_device::gamepad_message_cref_t ) ;
        so_device::result transmit_message( so_device::gamepad_message_cref_t ) ;

        so_device::result trigger_component( so_device::key_cref_t, trigger_funk_t ) ;

    public:

        void_t update( gamepad_messages_ref_t ) ;

    public:

        so_device::result add_component( so_device::key_cref_t, input_component_logic_cref_t,
            so_device::so_component::iinput_component_ptr_t ) ;

        so_device::result add_component( so_device::key_cref_t, output_component_logic_cref_t,
            so_device::so_component::ioutput_component_ptr_t ) ;

        so_device::result register_notification( iuser_notify_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;

    };
    so_typedef( gamepad_device ) ;
}

#endif
