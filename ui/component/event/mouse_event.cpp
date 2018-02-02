//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "mouse_event.h"

#include <snakeoil/device/devices/mouse/three_button_mouse.h>

using namespace so_ui ;
using namespace so_ui::so_component ;

//*****************************************************************************************
mouse_event::mouse_event( void_t )
{

}

//*****************************************************************************************
mouse_event::mouse_event( this_rref_t rhv )
{
    _mouse_events = std::move( rhv._mouse_events ) ;
    _global_id = rhv._global_id ;
}

//*****************************************************************************************
mouse_event::~mouse_event( void_t )
{

}

//*****************************************************************************************
mouse_event::this_ptr_t mouse_event::create( so_memory::purpose_cref_t p )
{
    return this_t::create( this_t(), p ) ;
}

//*****************************************************************************************
mouse_event::this_ptr_t mouse_event::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_ui::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************
size_t mouse_event::add_event_receiver( mouse_event_funk_t funk )
{
    mouse_event_data_t d ;
    d.mevent = funk ;
    d.id = _global_id++ ;
    _mouse_events.push_back( d ) ;

    return d.id ;
}

//*************************************************************************
void_t mouse_event::for_each_mouse_event_trigger( mouse_event_trigger_t funk_in )
{
    for( auto & item : _mouse_events )
    {
        funk_in( item.mevent ) ;
    }
}

//*************************************************************************
void_t mouse_event::remove_event_receiver( size_t const id )
{
    auto const iter = std::find_if( _mouse_events.begin(), _mouse_events.end(), [&] ( mouse_event_data_cref_t d )
    {
        return d.id == id ;
    } ) ;

    if( iter == _mouse_events.end() )
        return ;

    _mouse_events.erase( iter ) ;
}

//*****************************************************************************************
void_t mouse_event::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//*****************************************************************************************
void_t mouse_event::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*****************************************************************************************