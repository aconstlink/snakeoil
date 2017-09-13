//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "imouse_device.h"

#include "../../component/buttons/three_button.h"
#include "../../component/buttons/button_state.h"

#include <snakeoil/std/container/map.hpp>
#include <snakeoil/std/container/vector.hpp>

#include <snakeoil/math/vector/vector2.hpp>

namespace so_device
{
    class SNAKEOIL_DEVICE_API three_button_mouse : public imouse_device
    {
        so_this_typedefs( three_button_mouse ) ;
        
    private:

        typedef so_std::map< so_device::three_button, so_device::button_state > __three_button_states_t ;
        so_typedefs( __three_button_states_t, three_button_states ) ;

        typedef so_std::vector< std::pair< three_button, button_state > > __three_button_history_t ;
        so_typedefs( __three_button_history_t, three_button_history ) ;

        three_button_states_t _three_button_states ;
        three_button_history_t _three_button_history ;

        so_math::vec2f_t _global_position ;
        so_math::vec2f_t _local_position ;

    public:

        three_button_mouse( void_t ) ;
        three_button_mouse( this_cref_t ) = delete ;
        three_button_mouse( this_rref_t ) ;
        virtual ~three_button_mouse( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t add_mouse_event( so_device::three_button const, so_device::button_state const ) ;
        void_t set_global_position( so_math::vec2f_cref_t ) ;
        void_t set_local_position( so_math::vec2f_cref_t ) ;

    public:

        so_math::vec2f_t get_global_position( void_t ) const ;
        so_math::vec2f_t get_local_position( void_t ) const ;

        bool_t is_pressed( so_device::three_button const ) const ;
        bool_t is_pressing( so_device::three_button const ) const ;
        bool_t is_released( so_device::three_button const ) const ;


    public:

        void_t update( void_t ) ;

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( three_button_mouse ) ;
}