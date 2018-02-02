//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "three_button_mouse.h"

using namespace so_device ;


//*****************************************************************************************
three_button_mouse::three_button_mouse( void_t )
{

}

//*****************************************************************************************
three_button_mouse::three_button_mouse( this_rref_t rhv )
{
    _global_position = rhv._global_position ;
    _local_position = rhv._local_position ;
    _three_button_states = std::move( rhv._three_button_states ) ;
    _three_button_history = std::move( rhv._three_button_history ) ;
}

//*****************************************************************************************
three_button_mouse::~three_button_mouse( void_t )
{

}

//*****************************************************************************************
three_button_mouse::this_ptr_t three_button_mouse::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_device::memory::alloc( std::move( rhv ), p ) ;
}

//*****************************************************************************************
void_t three_button_mouse::destroy( this_ptr_t ptr )
{
    so_device::memory::dealloc( ptr ) ;
}

//*****************************************************************************************
void_t three_button_mouse::add_mouse_event( so_device::three_button const tb, so_device::button_state const bs )
{
    auto iter = _three_button_states.find( tb ) ;
    if( iter == _three_button_states.end() )
    {
        _three_button_states[ tb ] = bs ;
        _three_button_history.push_back( std::make_pair( tb, bs ) ) ;
    }
    else
    {
        if( iter->second == button_state::pressing && bs != button_state::pressed )
            iter->second = bs ;
    }
}

//*****************************************************************************************
void_t three_button_mouse::set_global_position( so_math::vec2f_cref_t v )
{
    _global_position = v ;
}

//*****************************************************************************************
void_t three_button_mouse::set_local_position( so_math::vec2f_cref_t v )
{
    _local_position = v ;
}

//*****************************************************************************************
void_t three_button_mouse::scroll_down( void_t )
{
    --_scroll ;
}

//*****************************************************************************************
void_t three_button_mouse::scroll_up( void_t )
{
    ++_scroll ;
}

//*****************************************************************************************
void_t three_button_mouse::scroll_reset( void_t )
{
    _scroll = 0 ;
}

//*****************************************************************************************
int_t three_button_mouse::get_scroll( void_t ) const
{
    return _scroll ;
}

//*****************************************************************************************
so_math::vec2f_t three_button_mouse::get_global_position( void_t ) const
{
    return _global_position ;
}

//*****************************************************************************************
so_math::vec2f_t three_button_mouse::get_local_position( void_t ) const
{
    return _local_position ;
}

//*****************************************************************************************
so_math::vec2f_t three_button_mouse::get_local_position( so_math::vec2f_cref_t pivot ) const
{
    return _local_position - pivot ;
}

//*****************************************************************************************
bool_t three_button_mouse::is_pressed( so_device::three_button const tb ) const
{
    auto const iter = _three_button_states.find( tb ) ;
    return iter != _three_button_states.end() ? iter->second == button_state::pressed : false ;
}

//*****************************************************************************************
bool_t three_button_mouse::is_pressing( so_device::three_button const tb ) const
{
    auto const iter = _three_button_states.find( tb ) ;
    return iter != _three_button_states.end() ? iter->second == button_state::pressing : false ;
}

//*****************************************************************************************
bool_t three_button_mouse::is_released( so_device::three_button const tb ) const
{
    auto const iter = _three_button_states.find( tb ) ;
    return iter != _three_button_states.end() ? iter->second == button_state::released : false ;
}

//*****************************************************************************************
void_t three_button_mouse::update( void_t )
{
    auto tmp_states = std::move( _three_button_states ) ;
    auto tmp_history = std::move( _three_button_history ) ;

    for( auto & item : tmp_states )
    {
        if( item.second == button_state::pressed || item.second == button_state::pressing )
        {
            item.second = button_state::pressing ;
            _three_button_states.insert( item ) ;
        }
    }

    for( auto const & item : tmp_history )
    {
        auto const iter = _three_button_states.find( item.first ) ;
        if( iter == _three_button_states.end() ) continue ;

        _three_button_history.push_back( *iter ) ;
    }

    _scroll = 0 ;
}

//*****************************************************************************************
void_t three_button_mouse::destroy( void_t )
{
    this_t::destroy( this ) ;
}