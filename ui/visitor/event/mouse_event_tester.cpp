//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "mouse_event_tester.h"

#include "../../node/layout/area_2d.h"
#include "../../node/event/input_event.h"

#include <snakeoil/collide/2d/bounds/aabb.hpp>
#include <snakeoil/device/devices/mouse/three_button_mouse.h>
#include <snakeoil/log/global.h>

using namespace so_ui ;
using namespace so_ui::so_visitor ;

//*************************************************************************
void_t mouse_event_tester::node_listener::on_move( so_ui::so_node::node_ptr_t nptr )
{
    _owner->_hit_old = dynamic_cast< so_ui::so_node::input_event_ptr_t >( nptr ) ;
}

//*************************************************************************
mouse_event_tester::mouse_event_tester( void_t )
{
    _listener = so_ui::memory::alloc( node_listener_t( this ), 
        "[mouse_event_tester::mouse_event_tester] : node_listener" ) ;
}

//*************************************************************************
mouse_event_tester::mouse_event_tester( this_rref_t rhv )
{
    so_move_member_ptr( _mptr, rhv ) ;
    so_move_member_ptr( _listener, rhv ) ;
    _listener->set_owner( this ) ;
}

//*************************************************************************
mouse_event_tester::~mouse_event_tester( void_t )
{
    if( so_core::is_not_nullptr( _hit_old ) )
        _hit_old->remove_listener( _listener ) ;

    if( so_core::is_not_nullptr( _listener ) )
        so_ui::memory::dealloc( _listener ) ;
}

//*************************************************************************
mouse_event_tester::this_ptr_t mouse_event_tester::create( so_memory::purpose_cref_t p )
{
    return this_t::create( this_t(), p ) ;
}

//*************************************************************************
mouse_event_tester::this_ptr_t mouse_event_tester::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_ui::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************
void_t mouse_event_tester::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//*************************************************************************
so_ui::result mouse_event_tester::visit( so_ui::so_node::area_2d_ptr_t aptr )
{
    // check if in range
    auto const area = aptr->get_area() ;

    so_collide::so_2d::aabb< float_t > const aabb( area.xy(), area.xy() + area.zw() ) ;

    bool_t const inside = aabb.is_inside( _mptr->get_local_position() ) ;

    if( so_core::is_not( inside ) )
        return so_ui::no_descent ;

    // compute mouse coords relative to hit node
    //_local_coords = 

    return so_ui::ok ;
}

//*************************************************************************
so_ui::result mouse_event_tester::post_visit( so_ui::so_node::area_2d_ptr_t )
{
    return so_ui::ok ;
}

//*************************************************************************
so_ui::result mouse_event_tester::visit( so_ui::so_node::input_event_ptr_t ie_ptr )
{
    _hit_new = ie_ptr ;

    return so_ui::ok ;
}

//*************************************************************************
so_ui::result mouse_event_tester::post_visit( so_ui::so_node::input_event_ptr_t )
{
    return so_ui::ok ;
}

//*************************************************************************
void_t mouse_event_tester::trigger_hit_node( void_t )
{
    so_ui::mouse_event_t me ;
    me.local_coords = _local_coords ;
    me.mptr = _mptr ;

    if( so_core::is_nullptr( _hit_old ) && so_core::is_nullptr( _hit_new ) )
    {
        return ;
    }
    // enter event
    else if( so_core::is_nullptr( _hit_old ) && so_core::is_not_nullptr( _hit_new ) )
    {
        // enter hit_new
        _hit_new->for_each_mouse_event_trigger( [&] ( so_ui::so_node::input_event_t::mouse_event_funk_t funk )
        {
            me.me_type = so_ui::mouse_event_type::enter ;
            funk( me ) ;
        } ) ;

        _hit_new->register_listener( _listener ) ;

        _hit_old = _hit_new ;
        _hit_new = nullptr ;
    }
    // leave event
    else if( so_core::is_not_nullptr( _hit_old ) && so_core::is_nullptr( _hit_new ) )
    {
        // leave hit_old
        _hit_old->for_each_mouse_event_trigger( [&] ( so_ui::so_node::input_event_t::mouse_event_funk_t funk )
        {
            me.me_type = so_ui::mouse_event_type::leave ;
            funk( me ) ;
        } ) ;

        _hit_old->remove_listener( _listener ) ;
        _hit_old = nullptr ;
    }
    else if( so_core::is_not_nullptr( _hit_old ) && so_core::is_not_nullptr( _hit_new ) )
    {
        // inside event
        if( _hit_new == _hit_old )
        {
            _hit_old->for_each_mouse_event_trigger( [&] ( so_ui::so_node::input_event_t::mouse_event_funk_t funk )
            {
                me.me_type = so_ui::mouse_event_type::inside ;
                funk( me ) ;
            } ) ;
        }
        // leave and enter events
        else
        {
            // leave hit_old
            _hit_old->for_each_mouse_event_trigger( [&] ( so_ui::so_node::input_event_t::mouse_event_funk_t funk )
            {
                me.me_type = so_ui::mouse_event_type::leave ;
                funk( me ) ;
            } ) ;

            // enter hit_new
            _hit_new->for_each_mouse_event_trigger( [&] ( so_ui::so_node::input_event_t::mouse_event_funk_t funk )
            {
                me.me_type = so_ui::mouse_event_type::enter ;
                funk( me ) ;
            } ) ;

            _hit_old->remove_listener( _listener ) ;
            _hit_new->register_listener( _listener ) ;
            _hit_old = _hit_new ;
            _hit_new = nullptr ;
        }
    }

    _hit_new = nullptr ;
}

//*************************************************************************
void_t mouse_event_tester::set_mouse( so_device::three_button_mouse_ptr_t ptr )
{
    _mptr = ptr ;
}

//*************************************************************************
void_t mouse_event_tester::destroy( void_t )
{
    this_t::destroy( this ) ;
}