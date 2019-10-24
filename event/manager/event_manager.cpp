
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
    so_thread::mrsw_t::writer_lock_t const wl( _named_mtx ) ;

    auto const iter = std::find_if( _named.begin(), _named.end(), [&]( named_trigger_cref_t item )
    {
        return name == item.name ;
    } ) ;
        
    if( iter != _named.end() )
    {
        auto const iiter = std::find_if( iter->triggers.begin(), iter->triggers.end(), [&] ( itrigger_ptr_t ptr )
        {
            return ptr == tptr ;
        } ) ;

        // only allow one particular trigger in the list
        if( iiter != iter->triggers.end() )
        {
            return so_event::result::ok ;
        }

        iter->triggers.emplace_back( tptr ) ;
        return so_event::result::ok ;
    }

    {
        this_t::named_trigger_t nt ;
        nt.name = name ;
        nt.triggers.emplace_back( tptr ) ;

        _named.emplace_back( std::move( nt ) ) ;
    }

    return so_event::result::ok ;
}

//******************************************************************
so_event::result event_manager::add_trigger( this_t::time_point_cref_t tp, so_event::itrigger_ptr_t tptr )
{
    so_thread::mrsw_t::writer_lock_t const wl( _timed_mtx ) ;

    auto const iter = std::find_if( _timed.begin(), _timed.end(), [&] ( timed_trigger_cref_t item )
    {
        return tp == item.tp ;
    } ) ;

    if( iter != _timed.end() )
    {
        auto const iiter = std::find_if( iter->triggers.begin(), iter->triggers.end(), [&] ( itrigger_ptr_t ptr )
        {
            return ptr == tptr ;
        } ) ;

        // only allow one particular trigger in the list
        if( iiter != iter->triggers.end() )
        {
            return so_event::result::ok ;
        }

        iter->triggers.emplace_back( tptr ) ;

        return so_event::result::ok ;
    }

    {
        this_t::timed_trigger_t tt ;
        tt.tp = tp ;
        tt.triggers.emplace_back( tptr ) ;

        _timed.emplace_back( std::move( tt ) ) ;
    }

    return so_event::result::ok ;
}

//******************************************************************
so_event::result event_manager::remove_trigger( so_event::itrigger_ptr_t tptr )
{
    // timed trigger
    {
        so_thread::mrsw_t::writer_lock_t const wl( _timed_mtx ) ;        

        for( auto & item : _timed )
        {
            auto const iter = std::find_if( item.triggers.begin(), item.triggers.end(), [&] ( itrigger_ptr_t ptr )
            {
                return ptr == tptr ;
            } ) ;

            item.triggers.erase( iter ) ;
        }
    }

    // named trigger
    {
        so_thread::mrsw_t::writer_lock_t const wl( _named_mtx ) ;

        for( auto & item : _named )
        {
            auto const iter = std::find_if( item.triggers.begin(), item.triggers.end(), [&] ( itrigger_ptr_t ptr )
            {
                return ptr == tptr ;
            } ) ;

            item.triggers.erase( iter ) ;
        }
    }

    return so_event::result::ok ;
}

//******************************************************************
so_event::result event_manager::trigger( so_event::name_cref_t name )
{
    so_thread::mrsw_t::reader_lock_t const rl( _named_mtx ) ;

    auto const iter = std::find_if( _named.begin(), _named.end(), [&] ( this_t::named_trigger_cref_t item ) 
    { 
        return item.name == name ;
    } ) ;

    if( iter == _named.end() ) 
        return so_event::result::name_not_found ;

    for( auto * ptr : iter->triggers )
    {
        ptr->on_event( so_event::event_info::triggered ) ;
    }

    return so_event::result::ok ;
}

//******************************************************************
so_event::result event_manager::update( so_core::clock_t::time_point const & tp )
{
    so_thread::mrsw_t::reader_lock_t const rl( _timed_mtx ) ;

    // 1. coarse check
    {
        if( _timed.size() == 0 ) 
        {
            _timed_index = size_t( -1 ) ;
            return so_event::result::ok ;
        }
    }
    
    size_t idx = _timed_index == size_t(-1) ? 0 : _timed_index ;

    // 2. update index
    {
        while( true ) 
        {
            auto const & item = _timed[ idx ] ;
            
            // forwards
            if( tp >= item.tp )
            {
                size_t const idx_next = idx + 1 ;
                if( idx_next == _timed.size() )
                    break ;

                if( tp < _timed[ idx_next ].tp )
                    break ;

                idx = idx_next ;
            }
            // backwards
            else if( tp < item.tp )
            {
                size_t const idx_next = idx - 1 ;
                if( idx_next == size_t(-1) )
                    break ;

                if( tp >= _timed[ idx_next ].tp )
                    break ;

                idx = idx_next ;
            }
        }
    }

    // 3. evaluate
    {
        size_t const start_index = _timed_index ;

        // forwards
        if( idx >= start_index )
        {
            idx += _timed_index == size_t( -1 ) ? 1 : 0 ;

            for( size_t i=start_index+1; i<=idx; ++i )
            {
                for( auto * tptr : _timed[i].triggers )
                    tptr->on_event( so_event::event_info::triggered ) ;
            }
        }
        // backwards
        else if( idx < start_index )
        {
            for( size_t i = start_index; i > idx; --i )
            {
                for( auto* tptr : _timed[ i ].triggers )
                    tptr->on_event( so_event::event_info::reset ) ;
            }
        }
    }

    _timed_index = idx ;

    return so_event::result::ok ;
}

//******************************************************************