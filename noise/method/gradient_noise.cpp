//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gradient_noise.h"

#include "../permutation/permutation_table.h"

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/utility/fn.hpp>
#include <snakeoil/math/constants.h>

using namespace so_noise ;

//***********************************************************************************
gradient_noise::this_ptr_t gradient_noise::create( this_rref_t rhv, std::string const & purpose ) 
{
    return so_noise::memory::alloc( std::move(rhv), purpose ) ;
}

//***********************************************************************************
void_t gradient_noise::destroy( this_ptr_t ptr ) 
{
    so_noise::memory::dealloc( ptr ) ;
}

//***********************************************************************************
gradient_noise::gradient_noise( uint_t seed, uint_t bit, uint_t num_mixes )
{
    _perm_ptr = permutation_table::create( permutation_table(seed, bit, num_mixes), 
        "[gradient_noise::gradient_noise] : permutation_table" ) ;

    _noises.resize( this_t::get_num_entries() ) ; 

    bit = _perm_ptr->get_bit() ;

    srand( seed ) ;
    for( uint_t i=0; i<get_num_entries(); ++i )
    {
        uint_t const value_limit = (1u<<bit)-1u ;

        float_t const z = -((float_t)((rand() & value_limit)/(double_t)value_limit)) * 
            2.0f + 1.0f ;

        float_t const r = so_math::fn<float_t>::sqrt( 1.0f - z*z ) ;
        float_t const theta = 2.0f * so_math::constants::pi<float_t>() * 
            ((float_t)((rand()& value_limit)/(double_t)value_limit));

        _noises[i] = so_math::vec3f_t( 
            r*so_math::fn<float_t>::cos(theta), 
            r*so_math::fn<float_t>::sin(theta), 
            z ) ;
    }
}


//***********************************************************************************
gradient_noise::gradient_noise( this_rref_t rhv )
{
    _noises = std::move(rhv._noises) ;
    _perm_ptr = rhv._perm_ptr ;
    rhv._perm_ptr = nullptr ;
}

//***********************************************************************************
gradient_noise::~gradient_noise( void_t )
{
    if( _perm_ptr != nullptr ) 
        _perm_ptr->destroy() ;
}

//***********************************************************************************
float_t gradient_noise::lattice( uint_t i0, float_t f1 ) const 
{
    so_math::vec3f_t const n = _noises[_perm_ptr->permute_at(i0)] ;
    return n.x() * f1  ;
}

//***********************************************************************************
float_t gradient_noise::lattice( uint_t i0, uint_t i1, float_t f1, float_t f2 ) const
{
    so_math::vec3f_t const n = _noises[_perm_ptr->permute_at(i0, i1)] ;
    return n.x() * f1 + n.y() * f2 ;
}

//***********************************************************************************
float_t gradient_noise::lattice( uint_t i0, uint_t i1, uint_t i2, float_t f1, float_t f2, float_t f3 ) const
{
    so_math::vec3f_t const n = _noises[_perm_ptr->permute_at(i0, i1, i2)] ;
    return n.x() * f1 + n.y() * f2 + n.z() * f3 ;
}

//***********************************************************************************
float_t gradient_noise::noise( float_t x ) const  
{
    const uint_t ix = (uint_t)so_math::fn<float_t>::floor(x) ;

    const float_t fx0 = x - (int_t)ix ;

    const float_t fx1 = fx0 - 1.0f ;

    const float_t wx = so_math::fn<float_t>::smooth_step(fx0) ;  

    const float_t g00 = this_t::lattice( ix, fx0 ) ;
    const float_t g10 = this_t::lattice( ix+1, fx1 ) ;

    return so_math::fn<float_t>::mix(wx, g00, g10) ;
}

//***********************************************************************************
float_t gradient_noise::noise( float_t x, float_t y ) const 
{
    const uint_t ix = (uint_t)so_math::fn<float_t>::floor(x) ;
    const uint_t iy = (uint_t)so_math::fn<float_t>::floor(y) ;

    const float_t fx0 = x - (int_t)ix ;
    const float_t fy0 = y - (int_t)iy ;

    const float_t fx1 = fx0 - 1.0f ;
    const float_t fy1 = fy0 - 1.0f ;

    const float_t wx = so_math::fn<float_t>::smooth_step(fx0) ;            
    const float_t wy = so_math::fn<float_t>::smooth_step(fy0) ;

    const float_t g00 = this_t::lattice( ix, iy, fx0, fy0 ) ;
    const float_t g10 = this_t::lattice( ix+1, iy, fx1, fy0 ) ;
    const float_t g01 = this_t::lattice( ix, iy+1, fx0, fy1 ) ;
    const float_t g11 = this_t::lattice( ix+1, iy+1, fx1, fy1 ) ;
            
    const float_t l1 = so_math::fn<float_t>::mix(wx, g00, g10) ;
    const float_t l2 = so_math::fn<float_t>::mix(wx, g01, g11) ;
    return so_math::fn<float_t>::mix(wy, l1, l2) ;
}

//***********************************************************************************
float_t gradient_noise::noise( float_t x, float_t y, float_t z ) const 
{
    const uint_t ix = (uint_t)so_math::fn<float_t>::floor(x) ;
    const uint_t iy = (uint_t)so_math::fn<float_t>::floor(y) ;
    const uint_t iz = (uint_t)so_math::fn<float_t>::floor(z) ;

    const float_t fx0 = x - (int_t)ix ;
    const float_t fy0 = y - (int_t)iy ;
    const float_t fz0 = z - (int_t)iz ;

    const float_t fx1 = fx0 - 1.0f ;
    const float_t fy1 = fy0 - 1.0f ;
    const float_t fz1 = fz0 - 1.0f ;

    const float_t wx = so_math::fn<float_t>::smooth_step(fx0) ;            
    const float_t wy = so_math::fn<float_t>::smooth_step(fy0) ;
    const float_t wz = so_math::fn<float_t>::smooth_step(fz0) ;

    const float_t g000 = this_t::lattice( ix, iy, iz, fx0, fy0, fz0 ) ;
    const float_t g100 = this_t::lattice( ix+1, iy, iz, fx1, fy0, fz0 ) ;
    const float_t g010 = this_t::lattice( ix, iy+1, iz, fx0, fy1, fz0 ) ;
    const float_t g110 = this_t::lattice( ix+1, iy+1, iz, fx1, fy1, fz0 ) ;
    const float_t g001 = this_t::lattice( ix, iy, iz+1, fx0, fy0, fz1 ) ;
    const float_t g101 = this_t::lattice( ix+1, iy, iz+1, fx1, fy0, fz1 ) ;
    const float_t g011 = this_t::lattice( ix, iy+1, iz+1, fx0, fy1, fz1 ) ;
    const float_t g111 = this_t::lattice( ix+1, iy+1, iz+1, fx1, fy1, fz1 ) ;

    const float_t l1 = so_math::fn<float_t>::mix(wx, g000, g100) ;
    const float_t l2 = so_math::fn<float_t>::mix(wx, g010, g110) ;
    const float_t l3 = so_math::fn<float_t>::mix(wx, g001, g101) ;
    const float_t l4 = so_math::fn<float_t>::mix(wx, g011, g111) ;

    const float_t l5 = so_math::fn<float_t>::mix(wy, l1, l2) ;
    const float_t l6 = so_math::fn<float_t>::mix(wy, l3, l4) ;

    return so_math::fn<float_t>::mix(wz, l5, l6) ;
}

//***********************************************************************************
void_t gradient_noise::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//***********************************************************************************
uint_t gradient_noise::get_num_entries( void_t ) const
{ 
    return _perm_ptr->get_num_entries() ; 
}

