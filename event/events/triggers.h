#pragma once

#include "itrigger.h"

namespace so_event
{
    class SNAKEOIL_EVENT_API flagged_trigger : public itrigger
    {
        so_this_typedefs( flagged_trigger ) ;

    private:

        so_event::event_info _dir = 
            so_event::event_info::invalid ;

    public:

        flagged_trigger( void_t ){}
        flagged_trigger( this_cref_t rhv ) : 
            _dir(rhv._dir){}
        flagged_trigger( this_rref_t rhv ) :
            _dir( rhv._dir ) {}
        virtual ~flagged_trigger( void_t ) {}

        static this_ptr_t create( so_memory::purpose_cref_t p )
        {
            return so_memory::global_t::alloc( this_t(), p ) ;
        }

        static void_t destroy( this_ptr_t ptr )
        {
            so_memory::global_t::dealloc( ptr ) ;
        }

    public:

        virtual void_t on_event( so_event::event_info dir ) 
        {
            _dir = dir ;
        }

    public:

        /// this is supposed to be called once by the entity 
        /// added the trigger. The current direction is invalidated.
        so_event::event_info swap_and_get( void_t ) 
        {
            auto const ret = _dir ;
            _dir = so_event::event_info::invalid ;
            return ret ;
        }
    };
    so_typedef( flagged_trigger ) ;
}