//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_DEVICES_KEYBOARD_ASCII_ASCII_KEYBOARD_DEVICE_H_
#define _SNAKEOIL_DEVICE_DEVICES_KEYBOARD_ASCII_ASCII_KEYBOARD_DEVICE_H_

#include "ikeyboard_device.h"

#include "../../component/keys/key_state.h"
#include "../../component/keys/ascii_key.h"

#include <snakeoil/std/container/map.hpp>
#include <snakeoil/std/container/vector.hpp>

#include <functional>

namespace so_device
{
    class SNAKEOIL_DEVICE_API ascii_keyboard : public ikeyboard_device
    {
        so_this_typedefs( ascii_keyboard ) ;

    public:

        typedef std::function< void_t ( ascii_key const, key_state const ) > for_each_funk_t ;

    private:

        typedef so_std::map< ascii_key, key_state > __ascii_key_states_t ;
        so_typedefs( __ascii_key_states_t, ascii_key_states ) ;

        typedef so_std::vector< std::pair< ascii_key, key_state > > __ascii_key_history_t ;
        so_typedefs( __ascii_key_history_t, ascii_key_history ) ;

        /// stores the current ascii key state
        ascii_key_states_t _ascii_key_states ;

        /// stores all events for one cycle
        ascii_key_history_t _ascii_key_history ;

    public:

        ascii_keyboard( void_t ) ;
        ascii_keyboard( this_cref_t ) = delete ;
        ascii_keyboard( this_rref_t ) ;
        virtual ~ascii_keyboard( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t add_key_event( ascii_key const, key_state const ) ;
        void_t update( void_t ) ;

    public:

        bool_t is_pressed( ascii_key const ) const ;
        bool_t is_pressing( ascii_key const ) const ;
        bool_t is_released( ascii_key const ) const ;

        void_t for_each_key( for_each_funk_t ) const ;
        
    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( ascii_keyboard ) ;
}

#endif