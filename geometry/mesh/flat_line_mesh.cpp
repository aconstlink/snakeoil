//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "flat_line_mesh.h"

using namespace so_geo ;

//*************************************************************************************
flat_line_mesh::flat_line_mesh( void_t ) 
{
}

//*************************************************************************************
flat_line_mesh::flat_line_mesh( this_rref_t rhv ) 
{
    indices = std::move( rhv.indices ) ;
    positions = std::move( rhv.positions ) ;
    position_format = rhv.position_format ;
}

//*************************************************************************************
flat_line_mesh::~flat_line_mesh( void_t ) 
{
}

//*************************************************************************************
flat_line_mesh::this_ref_t flat_line_mesh::operator = ( this_rref_t rhv )
{
    indices = std::move( rhv.indices ) ;
    positions = std::move( rhv.positions ) ;
    position_format = rhv.position_format ;

    return *this ;
}

//*************************************************************************************
flat_line_mesh::this_ptr_t flat_line_mesh::create( so_memory::purpose_cref_t p ) 
{
    return so_geo::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
flat_line_mesh::this_ptr_t flat_line_mesh::create( this_rref_t rhv, so_memory::purpose_cref_t p) 
{
    return so_geo::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t flat_line_mesh::destroy( this_ptr_t ptr ) 
{
    so_geo::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t flat_line_mesh::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
flat_line_mesh::this_t flat_line_mesh::repeat_full( size_t const times ) const
{
    this_t obj ;

    obj.position_format = position_format ;

    size_t num_positions = positions.size() ;
    
    if( position_format == so_geo::vector_component_format::xy )
    {
        num_positions /= 2 ;
    }
    else if( position_format == so_geo::vector_component_format::xyz )
    {
        num_positions /= 3 ;
    }

    obj.positions.resize( positions.size() * times ) ;
    obj.indices.resize( indices.size() * times ) ;

    for( size_t i = 0; i < positions.size()*times; ++i )
    {
        size_t const ii = i%positions.size() ;
        obj.positions[ i ] = positions[ ii ] ;
    }

    for( size_t i = 0; i < indices.size()*times; ++i )
    {
        size_t const ii = i % indices.size() ;
        size_t const offset = (i / indices.size()) * num_positions ;
        obj.indices[ i ] = indices[ ii ] + uint_t(offset) ;
    }

    return std::move( obj ) ;
}
