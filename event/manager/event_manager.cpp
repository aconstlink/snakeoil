
#include "event_manager.h"

#include <snakeoil/thread/semaphore_guard.hpp>

using namespace so_event ;

//******************************************************************
event_manager::event_manager( void_t ) 
{
}

//******************************************************************
event_manager::event_manager( this_rref_t rhv )
{
    _timed = std::move( rhv._timed ) ;
    _named = std::move( rhv._named ) ;
}

//******************************************************************
event_manager::~event_manager( void_t )
{
}

//******************************************************************
so_event::result event_manager::add_trigger( so_event::name_cref_t name, so_event::itrigger_ptr_t tptr )
{
    // search name
    { 
        so_thread::mrsw_t::writer_lock_t const wl( _timed_lock ) ;

        auto const iter = std::find_if( _named.begin(), _named.end(), [&]( named_trigger_cref_t item )
        {
            return name == item.name ;
        } ) ;
        
        if( iter == _named.end() )
        {
            
            return so_event::result::ok ;
        }
    }


    //iter->triggers.emplace_back( tptr ) ;

    return so_event::result::ok ;
}

//******************************************************************
so_event::result event_manager::add_trigger( this_t::time_point_cref_t, so_event::itrigger_ptr_t )
{
    return so_event::result::ok ;
}

//******************************************************************
so_event::result event_manager::remove_trigger( so_event::itrigger_ptr_t )
{
    return so_event::result::ok ;
}

//******************************************************************
so_event::result event_manager::trigger( so_event::name_cref_t )
{
    return so_event::result::ok ;
}

//******************************************************************
so_event::result event_manager::update( float_t const dt )
{
    return so_event::result::ok ;
}

//******************************************************************