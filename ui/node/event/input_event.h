//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../decorator.h"
#include "../../structs/mouse_event.h"

#include <snakeoil/device/protos.h>
#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/std/container/vector.hpp>

namespace so_ui
{
    namespace so_node
    {
        class SNAKEOIL_UI_API input_event : public decorator
        {
            typedef decorator base_t ;
            so_this_typedefs( input_event ) ;
            
        public:

            typedef std::function< void_t ( mouse_event_cref_t ) > mouse_event_funk_t ;
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

            input_event( void_t ) ;
            input_event( this_rref_t ) ;
            input_event( so_ui::so_node::iparent_ptr_t ) ;
            virtual ~input_event( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            void_t set_decorated( so_ui::so_node::node_ptr_t ) ;

        public:

            size_t add_event_receiver( mouse_event_funk_t ) ;
            void_t remove_event_receiver( size_t const ) ;

            void_t for_each_mouse_event_trigger( mouse_event_trigger_t ) ;

        public:

            virtual so_ui::result apply( so_ui::so_visitor::ivisitor_ptr_t ) ;
            virtual void_t destroy( void_t ) ;
        };
        so_typedef( input_event ) ;
    }
}
