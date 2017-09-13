//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "ascii_keyboard.h"

using namespace so_device ;

//**********************************************************************************************
ascii_keyboard::ascii_keyboard( void_t )
{

}

//**********************************************************************************************
ascii_keyboard::ascii_keyboard( this_rref_t )
{

}

//**********************************************************************************************
ascii_keyboard::~ascii_keyboard( void_t )
{

}

//**********************************************************************************************
ascii_keyboard::this_ptr_t ascii_keyboard::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_device::memory::alloc( std::move( rhv ), p ) ;
}

//**********************************************************************************************
void_t ascii_keyboard::destroy( this_ptr_t ptr )
{
    so_device::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
void_t ascii_keyboard::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//**********************************************************************************************
void_t ascii_keyboard::add_key_event( ascii_key const ak, key_state const ks )
{
    auto iter = _ascii_key_states.find( ak ) ;
    if( iter == _ascii_key_states.end() )
    {
        _ascii_key_states[ ak ] = ks ;
        _ascii_key_history.push_back( std::make_pair( ak, ks ) ) ;
    }
    else
    {
        if( iter->second == key_state::pressing && ks != key_state::pressed )
            iter->second = ks ;
    }
}

//**********************************************************************************************
void_t ascii_keyboard::update( void_t )
{
    auto tmp_states = std::move( _ascii_key_states ) ;
    auto tmp_history = std::move( _ascii_key_history ) ;

    for( auto & item : tmp_states )
    {
        if( item.second == key_state::pressed || item.second == key_state::pressing )
        {
            item.second = key_state::pressing ;
            _ascii_key_states.insert( item ) ;
        }
    }
    
    for( auto const & item : tmp_history )
    {
        auto const iter = _ascii_key_states.find( item.first ) ;
        if( iter == _ascii_key_states.end() ) continue ;

        _ascii_key_history.push_back( *iter ) ;
    }
}

//**********************************************************************************************
bool_t ascii_keyboard::is_pressed( ascii_key const ak ) const
{
    auto const iter = _ascii_key_states.find( ak ) ;
    return iter != _ascii_key_states.end() ? iter->second == key_state::pressed : false ;
}

//**********************************************************************************************
bool_t ascii_keyboard::is_pressing( ascii_key const ak ) const
{
    auto const iter = _ascii_key_states.find( ak ) ;
    return iter != _ascii_key_states.end() ? iter->second == key_state::pressing : false ;
}

//**********************************************************************************************
bool_t ascii_keyboard::is_released( ascii_key const ak ) const
{
    auto const iter = _ascii_key_states.find( ak ) ;
    return iter != _ascii_key_states.end() ? iter->second == key_state::released : false ;
}

//**********************************************************************************************