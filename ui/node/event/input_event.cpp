//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "input_event.h"
#include "../../visitor/ivisitor.h"

#include <snakeoil/log/global.h>

using namespace so_ui ;
using namespace so_ui::so_node ;

//*************************************************************************
input_event::input_event( void_t )
{}

//*************************************************************************
input_event::input_event( this_rref_t rhv ) : base_t( std::move( rhv ) )
{}

//*************************************************************************
input_event::input_event( so_ui::so_node::iparent_ptr_t pptr ) : base_t( pptr )
{}

//*************************************************************************
input_event::~input_event( void_t )
{}

//*************************************************************************
input_event::this_ptr_t input_event::create( so_memory::purpose_cref_t purp )
{
    return so_ui::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************
input_event::this_ptr_t input_event::create( this_rref_t rhv, so_memory::purpose_cref_t purp )
{
    return so_ui::memory::alloc( std::move( rhv ), purp ) ;
}

//*************************************************************************
void_t input_event::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//*************************************************************************
void_t input_event::set_decorated( so_ui::so_node::node_ptr_t nptr )
{
    base_t::set_decorated( nptr ) ;
}

//*************************************************************************
so_ui::result input_event::apply( so_ui::so_visitor::ivisitor_ptr_t vptr )
{
    auto res = vptr->visit( this ) ;

    base_t::traverse_decorated( vptr, res ) ;

    return vptr->post_visit( this ) ;
}

//*************************************************************************
size_t input_event::add_event_receiver( mouse_event_funk_t funk )
{
    mouse_event_data_t d ;
    d.mevent = funk ;
    d.id = _global_id++ ;
    _mouse_events.push_back( d ) ;

    return d.id ;
}

//*************************************************************************
void_t input_event::for_each_mouse_event_trigger( mouse_event_trigger_t funk_in )
{
    for( auto & item : _mouse_events )
    {
        funk_in( item.mevent ) ;
    }
}

//*************************************************************************
void_t input_event::remove_event_receiver( size_t const id )
{
    auto const iter = std::find_if( _mouse_events.begin(), _mouse_events.end(), [&] ( mouse_event_data_cref_t d )
    {
        return d.id == id ;
    } ) ;

    if( iter == _mouse_events.end() )
        return ;

    _mouse_events.erase( iter ) ;
}

//*************************************************************************
void_t input_event::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************
