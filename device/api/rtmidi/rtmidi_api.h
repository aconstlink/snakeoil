//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../imidi_api.h"
#include "../../devices/midi/midi_message.h"

#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/thread/mutex.h>

namespace so_device
{
    class SNAKEOIL_DEVICE_API rtmidi_api : public imidi_api
    {
        so_this_typedefs( rtmidi_api ) ;
            
    public:

        struct global_self
        {
            this_ptr_t self_ptr = nullptr ;
        };
        so_typedef( global_self ) ;

    private:


        struct data_pimpl ;
        so_typedef( data_pimpl ) ;

    private:

        struct store_data
        {
            so_this_typedefs( store_data );

            so_device::key_t key ;
            midi_device_ptr_t dev_ptr ;
            data_pimpl_ptr_t mptr ;

            this_ref_t operator = ( this_cref_t rhv ) = delete ;

            store_data( void_t ){}
            store_data( this_cref_t ) = delete ;
            store_data( this_rref_t rhv )
            {
                key = rhv.key;
                dev_ptr = rhv.dev_ptr;
                so_move_member_ptr( mptr, rhv ) ;
            }
            this_ref_t operator = ( this_rref_t rhv )
            {
                key = rhv.key;
                dev_ptr = rhv.dev_ptr;
                so_move_member_ptr( mptr, rhv ) ;
                return *this;
            }
        };
        so_typedef( store_data ) ;
        so_typedefs( so_std::vector< store_data_t >, midi_devices ) ;

    private:

        so_thread::mutex_t _mtx ;
            
        global_self_ptr_t _global_self_ptr = nullptr ;


        /// will contain a registered midi device
        midi_devices_t _devices ;

    private:
        so_typedefs( so_std::vector< so_device::imidi_notify_ptr_t >, midi_notifies ) ;

        so_thread::mutex_t _mtx_notifies ;
        midi_notifies_t _midi_notifies ;

    public:

        rtmidi_api( void_t ) ;
        rtmidi_api( this_rref_t ) ;
        virtual ~rtmidi_api( void_t ) ;

    public:
            
        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual void_t install_midi_notify( so_device::imidi_notify_ptr_t ) ;
            
        virtual void_t create_devices( so_device::imidi_module_ptr_t ) ;

        virtual void_t get_device_names( so_std::vector< so_std::string_t > & ) const ;

        virtual so_device::midi_device_ptr_t find_midi_device( so_device::key_cref_t ) ;
        virtual so_device::midi_device_ptr_t find_any_midi_device( void_t ) ;
            

        virtual void_t update_midi( void_t ) ;

    public:

        virtual void_t destroy( void_t ) ;

    public:

        //void_t handle_message( HMIDIIN, so_device::midi_message_cref_t ) ;

    private:

        void_t check_handle_for_device( size_t ) ;
        void_t check_for_new_devices( void_t ) ;

        so_device::result unregister_device( store_data_ref_t ) ;

    };

    so_typedef( rtmidi_api ) ;
}
