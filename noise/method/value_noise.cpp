//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "value_noise.h"

#include "../permutation/permutation_table.h"

#include <snakeoil/math/interpolation/interpolate.hpp>
#include <snakeoil/math/utility/fn.hpp>

using namespace so_noise ;

//***********************************************************************************
value_noise::this_ptr_t value_noise::create( this_rref_t rhv, std::string const & purpose ) 
{
    return so_noise::memory::alloc( std::move(rhv), purpose ) ;
}

//***********************************************************************************
void_t value_noise::destroy( this_ptr_t ptr ) 
{
    so_noise::memory::dealloc( ptr ) ;
}

//***********************************************************************************
value_noise::value_noise( uint_t seed, uint_t bit, uint_t num_mixes ) 
{
    _perm_ptr = permutation_table::create( permutation_table(seed, bit, num_mixes), 
        "[gradient_noise::gradient_noise] : permutation_table" ) ;
        
    _noises.resize(get_num_entries()) ;

    bit = _perm_ptr->get_bit() ;
    uint_t const value_limit = (1u<<bit)-1u ;
    
    srand( seed ) ;
    for( uint_t i=0; i<this_t::get_num_entries(); ++i )
    {
        _noises[i] = ((float_t)((rand() & value_limit)/(double_t)value_limit))*2.0f - 1.0f  ;
    }
}

//***********************************************************************************
value_noise::value_noise( this_rref_t rhv ) 
{
    _noises = std::move( rhv._noises ) ;
    _perm_ptr = rhv._perm_ptr ;
    rhv._perm_ptr = nullptr ;
}

//***********************************************************************************
value_noise::~value_noise( void_t ) 
{
    if( _perm_ptr != nullptr )
        _perm_ptr->destroy() ;
}

//***********************************************************************************
float_t value_noise::lattice( uint_t i0 ) const 
{
    return _noises[_perm_ptr->permute_at(i0)] ;
}

//***********************************************************************************
float_t value_noise::lattice( uint_t i0, uint_t i1 ) const 
{
    return _noises[_perm_ptr->permute_at(i0, i1)] ;
}

//***********************************************************************************
float_t value_noise::lattice( uint_t i0, uint_t i1, uint_t i2 ) const 
{
    return _noises[_perm_ptr->permute_at(i0, i1, i2)] ;
}

//***********************************************************************************
float_t value_noise::noise( float_t f0 ) const 
{
    int_t const i0 = int_t(std::floor(f0)) ;
    float_t const frac = f0 - float_t(i0) ;

    float_t const l0 = this_t::lattice(i0+0) ;
    float_t const l1 = this_t::lattice(i0+1) ;

    return so_math::interpolation<float_t>::linear
        ( l0, l1, so_math::fn<float_t>::smooth_step(frac) ) ;
}

//***********************************************************************************
float_t value_noise::noise( float_t f0, float_t f1 ) const 
{
    int_t const i0 = int_t(std::floor(f0)) ;
    float_t const frac0 = f0 - float_t(i0) ;

    int_t const i1 = int_t(std::floor(f1)) ;
    float_t const frac1 = f1 - float_t(i1) ;

    float_t const l00 = this_t::lattice(uint_t(i0+0), uint_t(i1+0)) ;
    float_t const l01 = this_t::lattice(uint_t(i0+1), uint_t(i1+0)) ;            
    float_t const l10 = this_t::lattice(uint_t(i0+0), uint_t(i1+1)) ;
    float_t const l11 = this_t::lattice(uint_t(i0+1), uint_t(i1+1)) ;
            
    float_t const v0 = so_math::interpolation<float_t>::linear
        ( l00, l01, so_math::fn<float_t>::smooth_step(frac0) ) ;

    float_t const v1 = so_math::interpolation<float_t>::linear
        ( l10, l11, so_math::fn<float_t>::smooth_step(frac0) ) ;

    return so_math::interpolation<float_t>::linear
        ( v0, v1, so_math::fn<float_t>::smooth_step(frac1) ) ;
}

//***********************************************************************************
float_t value_noise::noise( float_t /*f0*/, float_t /*f1*/, float_t /*f2*/ ) const 
{
    return 0.0f ;
}

//***********************************************************************************
uint_t value_noise::get_num_entries( void_t ) const 
{ 
    return _perm_ptr->get_num_entries() ; 
}

//***********************************************************************************
void_t value_noise::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

