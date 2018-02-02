//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "mouse_event_tester.h"

#include "../../component/event/mouse_event.h"
#include "../../component/transform/rect_transform.h"

#include <snakeoil/collide/2d/bounds/aabb.hpp>
#include <snakeoil/device/devices/mouse/three_button_mouse.h>
#include <snakeoil/log/global.h>

using namespace so_ui ;
using namespace so_ui::so_visitor ;

//*************************************************************************
mouse_event_tester::mouse_event_tester( void_t )
{}

//*************************************************************************
mouse_event_tester::mouse_event_tester( this_rref_t rhv )
{
    so_move_member_ptr( _mptr, rhv ) ;

    _dims = rhv._dims ;

    _area_stack = std::move( rhv._area_stack ) ;
}

//*************************************************************************
mouse_event_tester::~mouse_event_tester( void_t )
{}

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
so_ui::result mouse_event_tester::visit( so_ui::so_node::node_ptr_t nptr )
{
    so_math::vec4f_t dims = _area_stack.top() ;

    // rect
    {
        so_ui::so_component::rect_transform_ptr_t comp ;
        if( nptr->find_component( comp ) )
        {
            auto const ldims = comp->get_dims() ;

            dims = so_math::vec4f_t( ldims.xy() + dims.xy(), ldims.zw() ) ;
        }
    }

    _area_stack.push( dims ) ;

    // check hit
    {
        so_collide::so_2d::aabb< float_t > const aabb( dims.xy(), dims.xy() + dims.zw() ) ;

        bool_t const inside = aabb.is_inside( _mptr->get_local_position( so_math::vec2f_t( 0.0f ) ) * _dims ) ;

        if( so_core::is_not( inside ) )
            return so_ui::no_descent ;

        // compute mouse coords relative to hit node
        _local_coords = aabb.compute_relative( _mptr->get_local_position( so_math::vec2f_t( 0.0f ) ) * _dims ) ;
        _local_extend = aabb.get_extend() ;
    }

    // event
    {
        so_ui::so_component::mouse_event_ptr_t comp ;
        if( nptr->find_component( comp ) )
        {
            _hit_new = comp ;
        }
    }

    return so_ui::ok ;
}

//*************************************************************************
so_ui::result mouse_event_tester::post_visit( so_ui::so_node::node_ptr_t )
{
    _area_stack.pop() ;

    return so_ui::ok ;
}

//*************************************************************************
void_t mouse_event_tester::trigger_hit_node( void_t )
{
    so_ui::so_component::mouse_event::event_data_t me ;
    me.local_coords = _local_coords ;
    me.local_extend = _local_extend ;
    me.mptr = _mptr ;

    if( so_core::is_nullptr( _hit_old ) && so_core::is_nullptr( _hit_new ) )
    {
        return ;
    }
    // enter event
    else if( so_core::is_nullptr( _hit_old ) && so_core::is_not_nullptr( _hit_new ) )
    {
        // enter hit_new
        _hit_new->for_each_mouse_event_trigger( [&] ( so_ui::so_component::mouse_event_t::mouse_event_funk_t funk )
        {
            me.me_type = so_ui::so_component::mouse_event_t::event_type::enter ;
            funk( me ) ;
        } ) ;

        _hit_old = _hit_new ;
        _hit_new = nullptr ;
    }
    // leave event
    else if( so_core::is_not_nullptr( _hit_old ) && so_core::is_nullptr( _hit_new ) )
    {
        // leave hit_old
        _hit_old->for_each_mouse_event_trigger( [&] ( so_ui::so_component::mouse_event_t::mouse_event_funk_t funk )
        {
            me.me_type = so_ui::so_component::mouse_event_t::event_type::leave ;
            funk( me ) ;
        } ) ;

        _hit_old = nullptr ;
    }
    else if( so_core::is_not_nullptr( _hit_old ) && so_core::is_not_nullptr( _hit_new ) )
    {
        // inside event
        if( _hit_new == _hit_old )
        {
            _hit_old->for_each_mouse_event_trigger( [&] ( so_ui::so_component::mouse_event_t::mouse_event_funk_t funk )
            {
                me.me_type = so_ui::so_component::mouse_event_t::event_type::inside ;
                funk( me ) ;
            } ) ;
        }
        // leave and enter events
        else
        {
            // leave hit_old
            _hit_old->for_each_mouse_event_trigger( [&] ( so_ui::so_component::mouse_event_t::mouse_event_funk_t funk )
            {
                me.me_type = so_ui::so_component::mouse_event_t::event_type::leave ;
                funk( me ) ;
            } ) ;

            // enter hit_new
            _hit_new->for_each_mouse_event_trigger( [&] ( so_ui::so_component::mouse_event_t::mouse_event_funk_t funk )
            {
                me.me_type = so_ui::so_component::mouse_event_t::event_type::enter ;
                funk( me ) ;
            } ) ;

            _hit_old = _hit_new ;
            _hit_new = nullptr ;
        }
    }

    _hit_new = nullptr ;
}

//*************************************************************************
void_t mouse_event_tester::set_dimension( so_math::vec2f_cref_t dims )
{
    _dims = dims ;
    if( so_core::is_not( _area_stack.has_item() ) )
        _area_stack.push( so_math::vec4f_t( so_math::vec2f_t(), dims ) ) ;
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