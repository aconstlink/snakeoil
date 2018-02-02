//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../icomponent.h"

#include <snakeoil/device/protos.h>

#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/math/vector/vector2.hpp>

namespace so_ui
{
    namespace so_component
    {
        class SNAKEOIL_UI_API mouse_event : public so_ui::component_base< mouse_event >
        {
            so_this_typedefs( mouse_event ) ;

        public:

            enum class event_type
            {
                enter,
                leave,
                inside
            };

            struct event_data
            {
                event_type me_type ;
                so_math::vec2f_t local_coords ;
                so_math::vec2f_t local_extend ;
                so_device::three_button_mouse_ptr_t mptr ;
            };
            so_typedef( event_data ) ;


            typedef std::function< void_t ( event_data_cref_t ) > mouse_event_funk_t ;
            typedef std::function< void_t ( mouse_event_funk_t ) > mouse_event_trigger_t ;

        private:

            struct mouse_event_data
            {
                size_t id ;
                mouse_event_funk_t mevent ;
            } ;
            so_typedef( mouse_event_data ) ;

            so_typedefs( so_std::vector< mouse_event_data_t >, mouse_events ) ;

            mouse_events_t _mouse_events ;

            size_t _global_id = 0 ;

        public:

            mouse_event( void_t ) ;
            mouse_event( this_cref_t ) = delete ;
            mouse_event( this_rref_t ) ;
            virtual ~mouse_event( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            size_t add_event_receiver( mouse_event_funk_t ) ;
            void_t remove_event_receiver( size_t const ) ;

            void_t for_each_mouse_event_trigger( mouse_event_trigger_t ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( mouse_event ) ;
    }
}