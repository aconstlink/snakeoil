//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "vertex_shader.h"

using namespace so_shade ;
using namespace so_shade::so_shader ;

//*************************************************************************************
vertex_shader::vertex_shader( void_t ) 
{

}

//*************************************************************************************
vertex_shader::vertex_shader( so_shade::ascii_code_in_t code ) : _code(code)
{

}

//*************************************************************************************
vertex_shader::vertex_shader( this_cref_t rhv ) : shader( rhv )
{
    _code = ( rhv._code ) ;
    _pos_bind = ( rhv._pos_bind ) ;
    _nrm_bind = ( rhv._nrm_bind ) ;
    _texcoord_binds = ( rhv._texcoord_binds ) ;
    _object_binds = ( rhv._object_binds ) ;
    _cam_binds = ( rhv._cam_binds ) ;
}

//*************************************************************************************
vertex_shader::vertex_shader( this_rref_t rhv ) : shader( std::move(rhv) )
{
    _code = std::move( rhv._code ) ;
    _pos_bind = std::move( rhv._pos_bind ) ;
    _nrm_bind = std::move( rhv._nrm_bind ) ;
    _texcoord_binds = std::move( rhv._texcoord_binds ) ;
    _object_binds = std::move( rhv._object_binds ) ;
    _cam_binds = std::move( rhv._cam_binds ) ;
}

//*************************************************************************************
vertex_shader::this_ref_t vertex_shader::operator = ( this_cref_t rhv ) 
{
    shader::operator = ( rhv ) ;

    _code = (rhv._code) ;
    _pos_bind = (rhv._pos_bind) ;
    _nrm_bind = (rhv._nrm_bind) ;
    _texcoord_binds = (rhv._texcoord_binds) ;
    _object_binds = (rhv._object_binds) ;
    _cam_binds = (rhv._cam_binds) ;
    
    return *this ;
}

//*************************************************************************************
vertex_shader::~vertex_shader( void_t )
{

}

//*************************************************************************************
vertex_shader::this_ptr_t vertex_shader::create( this_cref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_shade::memory::alloc( rhv, p ) ;
}

//*************************************************************************************
vertex_shader::this_ptr_t vertex_shader::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t vertex_shader::destroy( this_ptr_t ptr ) 
{
    so_shade::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t vertex_shader::set_code( so_shade::ascii_code_in_t code )
{
    _code = code ;
}

//*************************************************************************************
ascii_code_cref_t vertex_shader::get_code( void_t )
{
    return _code ;
}

//*************************************************************************************
void_t vertex_shader::set( so_shade::so_shader::position_binding_in_t b )
{
    _pos_bind = b ;
}

//*************************************************************************************
void_t vertex_shader::set( so_shade::so_shader::normal_binding_in_t b )
{
    _nrm_bind = b ;
}

//*************************************************************************************
void_t vertex_shader::add( so_shade::so_shader::texcoord_binding_in_t b )
{
    _texcoord_binds.push_back( b ) ;
}

//*************************************************************************************
void_t vertex_shader::add( so_shade::so_shader::object_binding_in_t b ) 
{
    _object_binds.push_back( b ) ;
}

//*************************************************************************************
void_t vertex_shader::add( so_shade::so_shader::camera_binding_in_t b ) 
{
    _cam_binds.push_back( b ) ;
}

//*************************************************************************************
bool_t vertex_shader::get( so_shade::so_shader::position_binding_out_t bo ) const
{
    if( _pos_bind.var.name.empty() )
        return false ;

    bo = _pos_bind ;

    return true ;
}

//*************************************************************************************
bool_t vertex_shader::get( so_shade::so_shader::normal_binding_out_t bo ) const
{
    if( _nrm_bind.var.name.empty() )
        return false ;

    bo = _nrm_bind ;

    return true ;
}

//*************************************************************************************
size_t vertex_shader::get_num_texcood_bindings( void_t ) const
{
    return _texcoord_binds.size() ;
}
//*************************************************************************************
bool_t vertex_shader::get_texcoord_binding( size_t i, so_shade::so_shader::texcoord_binding_out_t bo ) const
{
    if( i >= _texcoord_binds.size() ) return false ;

    if( _texcoord_binds[i].var.name.empty() )
        return false ;

    bo = _texcoord_binds[i] ;

    return true ;
}
//*************************************************************************************
size_t vertex_shader::get_num_object_bindings( void_t ) const 
{
    return _object_binds.size() ;
}

//*************************************************************************************
bool_t vertex_shader::get_object_binding( size_t i, so_shade::so_shader::object_binding_out_t bo ) const
{
    if( i >= _object_binds.size() ) return false ;

    if( _object_binds[i].var.name.empty() )
        return false ;

    bo = _object_binds[i] ;

    return true ;
}

//*************************************************************************************
size_t vertex_shader::get_num_camera_bindings( void_t ) const
{
    return _cam_binds.size() ;
}

//*************************************************************************************
bool_t vertex_shader::get_camera_binding( size_t i, so_shade::so_shader::camera_binding_out_t bo ) const
{
    if( i >= _cam_binds.size() ) return false ;

    if( _cam_binds[i].var.name.empty() )
        return false ;

    bo = _cam_binds[i] ;

    return true ;
}

//*************************************************************************************
void_t vertex_shader::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
