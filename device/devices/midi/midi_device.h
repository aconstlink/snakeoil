//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_DEVICES_MIDI_MIDI_DEVICE_H_
#define _SNAKEOIL_DEVICE_DEVICES_MIDI_MIDI_DEVICE_H_

#include "../idevice.h"

#include "midi_message.h"
#include "midi_enums.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/std/string/string.hpp>

namespace so_device
{
    class SNAKEOIL_DEVICE_API midi_device : public idevice
    {
        so_this_typedefs( midi_device ) ;

        so_typedefs( so_std::vector<so_device::midi_message_t>, midi_msgs ) ;

    public: // input components typedefs

        typedef so_device::so_input::iinput_component_ptr_t in_comp_ptr_t ;
        typedef std::function< bool_t ( in_comp_ptr_t, midi_message_cref_t ) > midi_message_handle_funk_t ;
        typedef std::function< bool_t ( in_comp_ptr_t ) > follow_up_funk_t ;
        typedef std::function< void_t ( so_std::string_cref_t, in_comp_ptr_t ) > check_state_funk_t ;

        struct input_component_logic
        {
            midi_message_handle_funk_t midi_message_funk ;
            follow_up_funk_t follow_up_funk ;
        };
        so_typedef( input_component_logic ) ;

    public: // output components typedefs

        typedef so_device::so_output::ioutput_component_ptr_t out_comp_ptr_t ;
        typedef std::function< midi_output_result( out_comp_ptr_t, midi_message_ref_t ) > midi_out_message_handle_funk_t ;
       
        struct output_component_logic
        {
            midi_out_message_handle_funk_t handle_funk ;
        };
        so_typedef( output_component_logic ) ;

        typedef std::function< bool_t ( so_device::so_output::ioutput_component_ptr_t ) > 
            trigger_funk_t ;

        struct trigger_data
        {
            so_device::key_t key ;
            trigger_funk_t funk ;
        };
        so_typedef( trigger_data ) ;
        so_typedefs( so_std::vector<trigger_data_t>, trigger_datas ) ;

    public:

        struct device_info
        {
            size_t manufacturer_id = size_t(-1) ;
            size_t product_id = size_t(-1) ;
            size_t driver_version = size_t(-1) ;

            std::string device_name ;
        }; 
        so_typedef( device_info ) ;

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

        so_typedefs( so_std::vector<midi_message_t>, midi_messages ) ;
        so_typedefs( so_std::vector<iuser_notify_ptr_t>, notifys ) ;

    private:

        device_info _di ;

    private: // input components

        in_datas_t _in_datas ;
        in_datas_t _changed_comps ;

        so_thread::mutex_t _mtx_in ;
        midi_messages_t _in_msgs ;
        
    private: // output components

        out_datas_t _out_datas ;
        out_datas_t _triggered_component ;

        so_thread::mutex_t _mtx_out ;
        midi_messages_t _out_msgs ;


        so_thread::mutex_t _trigger_data_mtx ;
        trigger_datas_t _trigger_datas ;

    private:

        notifys_t _notifys ;

    public:

        midi_device( device_info_cref_t ) ;
        midi_device( this_cref_t ) = delete ;
        midi_device( this_rref_t ) ;
        virtual ~midi_device( void_t ) ;

    public:

        this_ref_t operator = ( this_rref_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:


        device_info_cref_t get_device_info( void_t ) const ;
        
    public:

        void_t clear( void_t ) ;

    public:

        /// allows to receive raw midi messages
        so_device::result receive_message( midi_message_cref_t ) ;

        /// allows to transmit raw midi message
        so_device::result transmit_message( midi_message_cref_t ) ;

        so_device::result trigger_component( so_device::key_cref_t, trigger_funk_t ) ;

    public:

        void_t update( midi_messages_ref_t ) ;

    public:

        so_device::result add_component( so_device::key_cref_t, input_component_logic_cref_t, 
            so_device::so_input::iinput_component_ptr_t ) ;

        so_device::result add_component( so_device::key_cref_t, output_component_logic_cref_t,
            so_device::so_output::ioutput_component_ptr_t ) ;

        so_device::result register_notification( iuser_notify_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( midi_device ) ;
}

#endif
