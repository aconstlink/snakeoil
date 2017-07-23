//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "pinhole_lens.h"

#include <snakeoil/math/utility/3d/perspective_fov.hpp>

using namespace so_imex ;


//*************************************************************************************
pinhole_lens::this_ptr_t pinhole_lens::create( so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
pinhole_lens::this_ptr_t pinhole_lens::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t pinhole_lens::destroy( this_ptr_t ptr ) 
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
pinhole_lens::pinhole_lens( void_t ) 
{
}

//*************************************************************************************
pinhole_lens::pinhole_lens( this_rref_t rhv ) 
{
    _fov_horz = rhv._fov_horz ;
    _fov_vert = rhv._fov_vert ;
    _aspect = rhv._aspect ;
    _near = rhv._near ;
    _far = rhv._far ;
}

//*************************************************************************************
pinhole_lens::~pinhole_lens( void_t ) 
{
}

//*************************************************************************************
void_t pinhole_lens::set_far( float_t v ) 
{
    _far = v ;
}

//*************************************************************************************
float_t pinhole_lens::get_far( void_t ) const
{
    return _far ;
}

//*************************************************************************************
void_t pinhole_lens::set_near( float_t v ) 
{
    _near = v ;
}

//*************************************************************************************
float_t pinhole_lens::get_near( void_t ) const
{
    return _near ;
}

//*************************************************************************************
void_t pinhole_lens::set_aspect( float_t v ) 
{
    _aspect = v ;
}

//*************************************************************************************
float_t pinhole_lens::get_aspect( void_t ) const
{
    return _aspect ;
}

//*************************************************************************************
void_t pinhole_lens::set_fov_hv( float_t h, float_t v ) 
{
    _fov_horz = h ;
    _fov_vert = v ;

    _aspect = so_math::so_3d::perspective<float_t>::aspect_for_fov( so_math::vec2f_t(h,v) ) ; 
}

//*************************************************************************************
void_t pinhole_lens::set_fovh_aspect( float_t h, float_t a ) 
{
    _fov_horz = h ;
    _aspect = a ;

    _fov_vert = so_math::so_3d::perspective<float_t>::fovv_for_fovh_aspect( _fov_horz, a ) ;
}

//*************************************************************************************
void_t pinhole_lens::set_fovv_aspect( float_t v, float_t a ) 
{
    _fov_vert = v ;
    _aspect = a ;

    _fov_horz = so_math::so_3d::perspective<float_t>::fovh_for_fovv_aspect( _fov_vert, a ) ;
}

//*************************************************************************************
float_t pinhole_lens::get_fovh( void_t ) const
{
    return _fov_horz ;
}

//*************************************************************************************
float_t pinhole_lens::get_fovv( void_t ) const
{
    return _fov_vert ;
}

//*************************************************************************************
void_t pinhole_lens::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
