
#pragma once

#include "../result.h"

#include "../events/ievent.h"
#include "../events/itrigger.h"

#include <snakeoil/thread/mrsw.hpp>

namespace so_event
{
    class SNAKEOIL_EVENT_API event_manager
    {
        so_this_typedefs( event_manager ) ;

    public:

        so_typedefs( so_core::clock_t::time_point, time_point ) ;

    private:

        so_typedefs( so_std::vector< itrigger_ptr_t >, triggers ) ;

        struct named_trigger
        {
            so_thread::mutex_t mtx ;
            so_std::string_t name ;
            triggers_t triggers ;

            named_trigger( void_t ) {}

            named_trigger( named_trigger && rhv )
            {
                name = std::move( rhv.name ) ;
                triggers = std::move( rhv.triggers ) ;
            }
        };
        so_typedef( named_trigger ) ;
        so_typedefs( so_std::vector< named_trigger_t >, named ) ;

        struct timed_trigger
        {
            so_thread::mutex_t mtx ;
            so_core::clock_t::time_point tp ;
            triggers_t triggers ;

            timed_trigger( void_t ) {}

            timed_trigger( timed_trigger&& rhv )
            {
                tp = std::move( rhv.tp ) ;
                triggers = std::move( rhv.triggers ) ;
            }
        };
        so_typedef( timed_trigger ) ;
        so_typedefs( so_std::vector< timed_trigger_t >, timed ) ;
        
        
        so_thread::mrsw_t _timed_mtx ;
        timed_t _timed ;
        size_t _timed_index = size_t(-1) ;

        so_thread::mrsw_t _named_mtx ;
        named_t _named ;

    public:

        event_manager( void_t ) ;
        event_manager( this_cref_t ) = delete ;
        event_manager( this_rref_t ) ;
        ~event_manager( void_t ) ;

    public:

        so_event::result add_trigger( so_event::name_cref_t, so_event::itrigger_ptr_t ) ;
        so_event::result add_trigger( this_t::time_point_cref_t, so_event::itrigger_ptr_t ) ;
        
        so_event::result remove_trigger( so_event::itrigger_ptr_t ) ;

    public:

        /// trigger named trigger
        so_event::result trigger( so_event::name_cref_t ) ;

    public:

        /// trigger timed trigger and do all update
        so_event::result update( so_core::clock_t::time_point const & ) ;

    };
    so_typedef( event_manager ) ;
}