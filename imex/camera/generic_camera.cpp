//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "generic_camera.h"

#include "ilens.h"

using namespace so_imex ;

//*************************************************************************************
generic_camera::generic_camera( void_t ) 
{

}

//*************************************************************************************
generic_camera::generic_camera( this_rref_t rhv ) 
{
    _lenses = std::move( rhv._lenses ) ;

    _position = rhv._position ;
    _look = rhv._look ;
    _up = rhv._up ;
    _right = rhv._right ;
}

//*************************************************************************************
generic_camera::~generic_camera( void_t ) 
{
    for( auto * lptr : _lenses )
    {
        lptr->destroy() ;
    }
}

//*************************************************************************************
generic_camera::this_ptr_t generic_camera::create( so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( this_t(), p ) ;

}

//*************************************************************************************
generic_camera::this_ptr_t generic_camera::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t generic_camera::destroy( this_ptr_t ptr ) 
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t generic_camera::set_position( so_math::vec3f_cref_t v ) 
{
    _position = v ;
}

//*************************************************************************************
so_math::vec3f_cref_t generic_camera::get_position( void_t ) const 
{
    return _position ;
}

//*************************************************************************************
void_t generic_camera::set_up_vector( so_math::vec3f_cref_t v ) 
{
    _up = v ;
}

//*************************************************************************************
so_math::vec3f_cref_t generic_camera::get_up_vector( void_t ) const 
{
    return _up ;
}

//*************************************************************************************
void_t generic_camera::set_look( so_math::vec3f_cref_t v ) 
{
    _look = v ;
}

//*************************************************************************************
so_math::vec3f_cref_t generic_camera::get_look( void_t ) const 
{
    return _look ;
}

//*************************************************************************************
void_t generic_camera::set_right( so_math::vec3f_cref_t v ) 
{
    _right = v ;
}

//*************************************************************************************
so_math::vec3f_cref_t generic_camera::get_right( void_t ) const 
{
    return _right ;
}

//*************************************************************************************
size_t generic_camera::get_num_lenses( void_t ) const 
{
    return _lenses.size() ;
}

//*************************************************************************************
ilens_cptr_t generic_camera::get_lens( size_t i ) const 
{
    return i < _lenses.size() ? _lenses[i] : nullptr ;
}

//*************************************************************************************
so_imex::result generic_camera::add_lens( ilens_ptr_t lptr ) 
{
    if( lptr == nullptr )
        return so_imex::invalid_argument ;

    auto found = std::find( _lenses.begin(), _lenses.end(), lptr ) ;
    if( found != _lenses.end() ) 
        return so_imex::already_managed ;

    _lenses.push_back( lptr ) ;

    return so_imex::ok ;

}

//*************************************************************************************
void_t generic_camera::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
