//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "generic_camera.h"
#include "ilens.h"
#include "pinhole_lens.h"

using namespace so_gfx ;

//**********************************************************************************************
generic_camera::generic_camera( void_t ) noexcept
{}

//**********************************************************************************************
generic_camera::generic_camera( so_gfx::ilens_utr_t ptr ) noexcept 
{
    this_t::add_lens( ptr ) ;
}

//**********************************************************************************************
generic_camera::generic_camera( this_rref_t rhv ) noexcept 
{
    _trafo = rhv._trafo ;
    _lenses = std::move( rhv._lenses ) ;
}

//**********************************************************************************************
generic_camera::~generic_camera( void_t ) noexcept
{
    for( ilens_ptr_t l : _lenses )
    {
        l->destroy() ;
    }
}

//**********************************************************************************************
generic_camera::this_ptr_t generic_camera::create( so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( this_t(), p ) ;
}

//**********************************************************************************************
generic_camera::this_ptr_t generic_camera::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************************
void_t generic_camera::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
so_gfx::result generic_camera::add_lens( so_gfx::ilens_utr_t lptr ) 
{
    auto found = std::find( _lenses.begin(), _lenses.end(), lptr ) ;
    if( found != _lenses.end() ) 
        return so_gfx::invalid_argument ;

    _lenses.push_back( lptr ) ;

    return so_gfx::ok ;
}

//**********************************************************************************************
size_t generic_camera::get_num_lenses( void_t ) const
{
    return _lenses.size() ;
}

//**********************************************************************************************
ilens_ptr_t generic_camera::get_lens( size_t i )
{
    return i < _lenses.size() ? _lenses[i] : nullptr ;
}

//**********************************************************************************************
void_t generic_camera::transform_by( so_math::so_3d::trafof_cref_t trafo ) 
{
    _trafo = _trafo * trafo ;
}

//**********************************************************************************************
void_t generic_camera::set_transformaion( so_math::so_3d::trafof_cref_t trafo ) 
{
    _trafo = trafo ;
    for( auto * lptr : _lenses )
    {
        lptr->transform_by( trafo ) ;
    }
}

//**********************************************************************************************
void_t generic_camera::get_transformation( so_math::so_3d::trafof_ref_t trafo_out ) const
{
    trafo_out = _trafo ;
}

//**********************************************************************************************
void_t generic_camera::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//**********************************************************************************************
