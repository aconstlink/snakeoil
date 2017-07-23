//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "permutation_table.h"

#include <snakeoil/core/assert.h>

using namespace so_noise ;

//***********************************************************************************
permutation_table::this_ptr_t permutation_table::create( this_rref_t rhv, std::string const & purpose ) 
{
    return so_noise::memory::alloc(std::move(rhv), purpose ) ;
}

//***********************************************************************************
void_t permutation_table::destroy( this_ptr_t ptr ) 
{
    so_noise::memory::dealloc( ptr ) ;
}

//***********************************************************************************
permutation_table::permutation_table( uint_t seed, uint_t bit, uint_t num_mixes=1 )
{
    num_mixes = std::min( std::max( 1u, num_mixes), 10u ) ;
    _seed = seed == 0 ? 6273 : seed ;
    _bit = bit < 3 ? 8 : bit ;

    _upper_bound = (1<<_bit)-1 ;

    _indices.resize( this_t::get_num_entries() ) ;

    srand(_seed) ;
            
    for( uint_t i=0; i<this_t::get_num_entries(); ++i )
    {
        _indices[i] = i ;
    }

    // swap 
    // values by real index with 
    // values by random index.
    for( uint_t mix=0; mix<num_mixes; ++mix )
    {
        for( size_t i=0; i<this_t::get_num_entries(); ++i )
        {
            uint_t const prn = rand() & _upper_bound ;
            uint_t const index = _indices[i] ;
            _indices[i] = _indices[prn] ;
            _indices[prn] = index ;
        }
    }
}

//***********************************************************************************
permutation_table::permutation_table( this_cref_t rhv )
{
    _seed = rhv._seed ;
    _upper_bound = rhv._upper_bound ;
    _bit = rhv._bit ;
    _indices = rhv._indices ;
}

//***********************************************************************************
permutation_table::permutation_table( this_rref_t rhv )
{
    _seed = rhv._seed ;
    _upper_bound = rhv._upper_bound ;
    _bit = rhv._bit ;
    _indices = std::move( rhv._indices ) ;
}

//***********************************************************************************
permutation_table::~permutation_table( void_t )
{
}

//***********************************************************************************
uint_t permutation_table::permute_at( uint_t i0 ) const 
{
    uint_t const i = perm( i0 & _upper_bound ) ;
    return _indices[i] ;
}

//***********************************************************************************
uint_t permutation_table::permute_at( uint_t i0, uint_t i1 ) const 
{
    const uint_t i = perm( i0 & _upper_bound ) ;
    const uint_t j = perm( (i+i1) & _upper_bound ) ;
    return _indices[j] ;
}

//***********************************************************************************
uint_t permutation_table::permute_at( uint_t i0, uint_t i1, uint_t i2 ) const 
{
    const uint_t i = perm( i0 & _upper_bound ) ;
    const uint_t j = perm( (i+i1) & _upper_bound ) ;
    const uint_t k = perm( (j+i2) & _upper_bound ) ;
    return _indices[k] ;
}

//***********************************************************************************
uint_t permutation_table::get_num_entries( void_t ) const 
{ 
    return 1 << _bit ; 
}

//***********************************************************************************
uint_t permutation_table::get_bit( void_t ) const 
{ 
    return _bit ; 
}

//***********************************************************************************
void_t permutation_table::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//***********************************************************************************
uint_t permutation_table::perm( uint_t i ) const
{
    return _indices[i] ;
}

