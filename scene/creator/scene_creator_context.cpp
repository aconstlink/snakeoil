//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "scene_creator_context.h"

#include <snakeoil/shade/shader/effect.h>

using namespace so_scene ;


//*************************************************************************************
scene_creator_context::scene_creator_context( void_t ) 
{

}

//*************************************************************************************
scene_creator_context::scene_creator_context( this_rref_t rhv )
{
    _effects = std::move( rhv._effects ) ;
}

//*************************************************************************************
scene_creator_context::~scene_creator_context( void_t )
{
    for( auto item : _effects )
    {
        so_shade::so_shader::effect_t::destroy(item.second) ;
    }
}

//*************************************************************************************
bool_t scene_creator_context::reserve_effect( so_std::string_in_t key_in ) 
{
    so_thread::lock_guard_t lk( _mtx_effects ) ;
    auto iter = _effects.find( key_in ) ;
    if( iter != _effects.end() )
        return false ;

    _effects[key_in] = nullptr ;

    return true ;
}

//*************************************************************************************
bool_t scene_creator_context::set_reserved_effect( so_std::string_in_t key_in, 
    so_shade::so_shader::effect_ptr_t effect_in ) 
{
    so_thread::lock_guard_t lk( _mtx_effects ) ;
    auto iter = _effects.find( key_in ) ;
    if( iter == _effects.end() )
        return false ;

    if( so_core::is_not_nullptr(iter->second) )
        return false ;

    iter->second = effect_in ;

    return true ;
}

//*************************************************************************************
bool_t scene_creator_context::add_effect( 
    so_std::string_in_t key_in, so_shade::so_shader::effect_ptr_t effect_in )
{
    so_thread::lock_guard_t lk( _mtx_effects ) ;
    auto iter = _effects.find( key_in ) ;
    if( iter != _effects.end() )
        return false ;

    _effects[key_in] = effect_in ;

    return true ;
}

//*************************************************************************************
bool_t scene_creator_context::get_effect( 
    so_std::string_in_t key_in, so_shade::so_shader::effect_ptr_t & effect_out )
{
    so_thread::lock_guard_t lk( _mtx_effects ) ;
    auto iter = _effects.find( key_in ) ;
    if( iter == _effects.end() )
        return false ;

    effect_out = iter->second ;

    return true ;
}

//*************************************************************************************
