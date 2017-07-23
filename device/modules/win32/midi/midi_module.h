//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_MODULE_WIN32_MIDI_MODULE_H_
#define _SNAKEOIL_DEVICE_MODULE_WIN32_MIDI_MODULE_H_

#include "../../imidi_module.h"
#include "../../../devices/midi/midi_message.h"

#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/thread/mutex.h>

#include <windows.h>

namespace so_device
{
    namespace so_win32
    {
        class SNAKEOIL_DEVICE_API midi_module : public imidi_module
        {
            so_this_typedefs( midi_module ) ;
            
        public:

            struct global_self
            {
                this_ptr_t self_ptr = nullptr ;
            };
            so_typedef( global_self ) ;

        private:

            struct midi_data
            {
                so_this_typedefs( midi_data );

                HMIDIIN inh = NULL ;
                HMIDIOUT outh = NULL ;
                LPSTR in_buffer = NULL ;

                midi_data( void_t ) {}
                
                midi_data( this_rref_t rhv )
                {
                    *this = std::move(rhv) ;
                }

                midi_data( this_cref_t rhv )
                {
                    *this = rhv ;
                }

                this_ref_t operator = ( this_cref_t rhv )
                {
                    inh = rhv.inh;
                    outh = rhv.outh;
                    in_buffer = rhv.in_buffer;

                    return *this ;
                }

                this_ref_t operator = ( this_rref_t rhv )
                {
                    inh = rhv.inh ;
                    rhv.inh = NULL ;

                    outh = rhv.outh ;
                    rhv.outh = NULL ;

                    so_move_member_ptr( in_buffer, rhv );
                    
                    return *this ;
                }

            };
            so_typedef( midi_data ) ;

            so_typedefs( so_std::vector<midi_data_t>, midi_datas ) ;

            struct in_message
            {
                HMIDIIN inh ;
                midi_message message ;
            };
            so_typedef( in_message ) ;
            so_typedefs( so_std::vector<in_message>, in_messages ) ;

            struct store_data
            {
                so_this_typedefs( store_data );

                so_device::key_t key ;
                midi_device_ptr_t dev_ptr ;
                midi_data mdata ;

                this_ref_t operator = ( this_cref_t rhv )
                {
                    key = rhv.key;
                    dev_ptr = rhv.dev_ptr;
                    mdata = rhv.mdata;
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

            so_thread::mutex_t _mtx_in ;
            in_messages_t _ins ;

        public:

            midi_module( void_t ) ;
            midi_module( this_rref_t ) ;
            virtual ~midi_module( void_t ) ;

        public:
            
            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_device::result register_device( so_device::key_cref_t, 
                so_device::midi_device_ptr_t ) ;

            virtual so_device::result unregister_device( so_device::key_cref_t ) ;

            virtual so_device::midi_device_ptr_t find_device( so_device::key_cref_t ) ;

            virtual void_t update( void_t ) ;

        public:

            virtual void_t destroy( void_t ) ;

        public:

            void_t handle_message( HMIDIIN, so_device::midi_message_cref_t ) ;

        private:

            void_t check_handle_for_device( size_t ) ;
            void_t check_for_new_devices( void_t ) ;

            so_device::result unregister_device( store_data_ref_t ) ;

        };

        so_typedef( midi_module ) ;
    }
}

#endif
