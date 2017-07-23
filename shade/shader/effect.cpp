//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "effect.h"

#include <algorithm>

using namespace so_shade ;
using namespace so_shader ;

//*************************************************************************************
effect::effect( void_t ) 
{
}

//*************************************************************************************
effect::effect( this_rref_t rhv )
{
    *this = std::move( rhv ) ;
}

//*************************************************************************************
effect::~effect( void_t )
{
    so_shade::so_shader::vertex_shader_t::destroy( _vs_ptr ) ;
    so_shade::so_shader::pixel_shader_t::destroy( _ps_ptr ) ;
}

//*************************************************************************************
effect::this_ref_t effect::operator = ( this_rref_t rhv ) 
{
    so_move_member_ptr( _vs_ptr, rhv ) ;
    so_move_member_ptr( _ps_ptr, rhv ) ;
    so_move( _pixel_shader_texture_bindings, rhv ) ;

    return *this ;
}

//*************************************************************************************
effect::effect( this_cref_t rhv ) 
{
    *this = rhv ;
}

//*************************************************************************************
effect::this_ref_t effect::operator = ( this_cref_t rhv ) 
{
    _vs_ptr = so_shade::so_shader::vertex_shader_t::create( *rhv._vs_ptr,
        "[effect::effect] : vertex shader copy" ) ;

    _ps_ptr = so_shade::so_shader::pixel_shader_t::create( *rhv._ps_ptr,
        "[effect::effect] : pixel shader copy" ) ;

    _pixel_shader_texture_bindings = rhv._pixel_shader_texture_bindings ;

    return *this ;
}

//*************************************************************************************
effect::this_ptr_t effect::create( so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
effect::this_ptr_t effect::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t effect::destroy( this_ptr_t ptr )
{
    so_shade::memory::dealloc( ptr ) ;
}

//*************************************************************************************
bool_t effect::set_vertex_shader( so_shade::so_shader::vertex_shader_ptr_t vptr ) 
{
    if( so_core::is_not_nullptr(_vs_ptr) ) return false ;

    _vs_ptr = vptr ;
   
    return true ;
}

//*************************************************************************************
bool_t effect::set_pixel_shader( so_shade::so_shader::pixel_shader_ptr_t sptr ) 
{
    if( so_core::is_not_nullptr( _ps_ptr ) ) return false ;

    _ps_ptr = sptr ;

    return true ;
}

//*************************************************************************************
so_shade::so_shader::vertex_shader_ptr_t effect::get_vertex_shader( void_t ) 
{
    return _vs_ptr ;
}

//*************************************************************************************
so_shade::so_shader::pixel_shader_ptr_t effect::get_pixel_shader( void_t ) 
{
    return _ps_ptr ;
}

//*************************************************************************************
so_shade::so_shader::vertex_shader_cptr_t effect::get_vertex_shader( void_t ) const 
{
    return _vs_ptr ;
}

//*************************************************************************************
so_shade::so_shader::pixel_shader_cptr_t effect::get_pixel_shader( void_t ) const 
{
    return _ps_ptr ;
}

//*************************************************************************************
bool_t effect::add_pixel_shader_binding( so_shade::texture_variable_cref_t binding_point,
    so_std::string_cref_t user_data )
{
    texture_binding_data_t bd ;
    bd.binding_point = binding_point.name ;
    bd.user_data = user_data ;
    _pixel_shader_texture_bindings.push_back( bd ) ;

    return true ;
}

//*************************************************************************************
bool_t effect::get_pixel_shader_binding( so_shade::texture_variable_cref_t var, 
    so_std::string_out_t nout ) const
{
    auto const iter = std::find_if( _pixel_shader_texture_bindings.begin(), 
        _pixel_shader_texture_bindings.end(), [=]( texture_binding_data_cref_t d )
    {
        return d.binding_point == var.name ;
    } ) ;
    
    if( iter == _pixel_shader_texture_bindings.end() )
        return false ;

    nout = iter->user_data ;

    return true ;
}

//*************************************************************************************
void_t effect::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
