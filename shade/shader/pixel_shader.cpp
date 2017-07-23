//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "pixel_shader.h"

using namespace so_shade ;
using namespace so_shade::so_shader ;

//*************************************************************************************
pixel_shader::pixel_shader( void_t ) 
{

}

//*************************************************************************************
pixel_shader::pixel_shader( so_shade::ascii_code_in_t code ) : _code(code)
{

}

//*************************************************************************************
pixel_shader::pixel_shader( this_rref_t rhv ) : shader( std::move(rhv) )
{
    so_move( _code, rhv ) ;
    so_move( _tex_binds, rhv ) ;
}

//*************************************************************************************
pixel_shader::pixel_shader( this_cref_t rhv ) : shader( rhv )
{
    _code = rhv._code ;
    _tex_binds = rhv._tex_binds ;
}

//*************************************************************************************
pixel_shader::~pixel_shader( void_t )
{

}

//*************************************************************************************
pixel_shader::this_ref_t pixel_shader::operator = ( this_cref_t rhv ) 
{
    shader::operator = ( rhv ) ;
    _code = rhv._code ;
    _tex_binds = rhv._tex_binds ;
    return *this ;
}

//*************************************************************************************
pixel_shader::this_ptr_t pixel_shader::create( this_cref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_shade::memory::alloc( rhv, p ) ;
}

//*************************************************************************************
pixel_shader::this_ptr_t pixel_shader::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t pixel_shader::destroy( this_ptr_t ptr ) 
{
    so_shade::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t pixel_shader::set_code( so_shade::ascii_code_in_t code )
{
    _code = code ;
}

//*************************************************************************************
ascii_code_cref_t pixel_shader::get_code( void_t )
{
    return _code ;
}

//*************************************************************************************
void_t pixel_shader::add( so_shade::so_shader::texture_binding_in_t b ) 
{
    _tex_binds.push_back( b ) ;
}

//*************************************************************************************
size_t pixel_shader::get_num_texture_bindings( void_t ) const 
{
    return _tex_binds.size() ;
}

//*************************************************************************************
bool_t pixel_shader::get_texture_binding( size_t i, so_shade::so_shader::texture_binding_out_t bo ) const 
{
    if( i >= _tex_binds.size() ) return false ;

    if( _tex_binds[i].var.name.empty() )
        return false ;

    bo = _tex_binds[i] ;

    return true ;
}

//*************************************************************************************
void_t pixel_shader::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
