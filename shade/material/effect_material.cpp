//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "effect_material.h"

#include "../shader/effect.h"

using namespace so_shade ;
using namespace so_shade::so_material ;


//**********************************************************************************************
effect_material::effect_material( void_t ) 
{
}

//**********************************************************************************************
effect_material::effect_material( so_shade::so_shader::effect_ptr_t eptr )
{
    _effect_ptr = eptr ;
}

//**********************************************************************************************
effect_material::effect_material( this_rref_t rhv )
{
    so_move_member_ptr( _effect_ptr, rhv ) ;
}

//**********************************************************************************************
effect_material::~effect_material( void_t )
{
    if( so_core::is_not_nullptr(_effect_ptr) )
        _effect_ptr->destroy() ;
}

//**********************************************************************************************
effect_material::this_ptr_t effect_material::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************************
void_t effect_material::destroy( this_ptr_t ptr )
{
    so_shade::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
so_shade::so_shader::effect_ptr_t effect_material::set_effect( so_shade::so_shader::effect_ptr_t eptr )
{
    auto * tmp = _effect_ptr ;
    _effect_ptr = eptr ;
    return tmp ;
}

//**********************************************************************************************
so_shade::so_shader::effect_ptr_t effect_material::get_effect( void_t )
{
    return _effect_ptr ;
}

//**********************************************************************************************
void_t effect_material::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//**********************************************************************************************
