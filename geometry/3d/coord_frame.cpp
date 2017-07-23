//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "coord_frame.h"

#include "../mesh/flat_line_mesh.h"

using namespace so_geo ;
using namespace so_geo::so_3d ;

/// one vertex position for each corner
so_math::vec3f_t const positions[6] =
{

    // center
    so_math::vec3f_t( 0.0f, 0.0f, 0.0f ),   // 0
    
    // right 
    so_math::vec3f_t( 1.0f, 0.0f, 0.0f ),   // 1

    // center
    so_math::vec3f_t( 0.0f, 0.0f, 0.0f ),   // 2

    // up
    so_math::vec3f_t( 0.0f, 1.0f, 0.0f ),   // 3
    
    // center
    so_math::vec3f_t( 0.0f, 0.0f, 0.0f ),   // 4

    // front
    so_math::vec3f_t( 0.0f, 0.0f, 1.0f )   // 5

} ;
size_t const g_num_positions = 6 ;

/// frame wire
uint_t const position_indices[6] = 
{
    // right
    0,1,
    // up
    2,3,
    //  front
    4,5
} ;
size_t const g_num_indices = 6 ;

//*************************************************************************************
so_geo::result coord_frame::make( flat_line_mesh_ptr_t wptr, input_params_cref_t params_in ) 
{
    if( wptr == nullptr )
        return so_geo::invalid_argument ;

    so_geo::flat_line_mesh_ref_t awire = *wptr ;

    // positions
    {
        awire.position_format = so_geo::vector_component_format::xyz ;
        awire.positions.resize( g_num_positions * 3 ) ;

        for( size_t i=0; i<g_num_positions; ++i )
        {
            awire.positions[i*3+0] = positions[i].x() * params_in.scale.x() ;
            awire.positions[i*3+1] = positions[i].y() * params_in.scale.y() ;
            awire.positions[i*3+2] = positions[i].z() * params_in.scale.z() ;
        }

        awire.indices.resize( g_num_indices ) ;
        for( size_t i = 0; i < g_num_indices; ++i )
        {
            awire.indices[i] = position_indices[i] ;
        }
    }    
    
    return so_geo::ok ;
}
