//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "buffer_creator.h"

#include <snakeoil/gpu/enums/vertex_attributes.h>
#include <snakeoil/gpu/buffer/vertex_buffer.hpp>
#include <snakeoil/gpu/buffer/index_buffer.hpp>
#include <snakeoil/gpu/program/config.h>

#include <snakeoil/log/log.h>

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>

using namespace so_manager ;
using namespace so_manager::so_graphics ;

//*************************************************************************************
void_t buffer_creator::construct_from( so_geo::flat_line_mesh_cref_t awire ) 
{
    _pt = so_gpu::primitive_type::lines ;

    // num_vector_components
    size_t const nvc = so_geo::vector_component_format_to_number(awire.position_format) ;

    if( awire.position_format == so_geo::vector_component_format::xyz )
    {
        struct vertex 
        {
            so_math::vec3f_t pos ;
        };
        so_typedefs( so_gpu::vertex_buffer<vertex>, vb ) ;
        so_typedefs( so_gpu::index_buffer<uint_t>, ib ) ;

        vb_ptr_t vb = vb_t::create( "[so_manager::mesh::construct_from] : vertex buffer" ) ;
        ib_ptr_t ib = ib_t::create( "[so_manager::mesh::construct_from] : index buffer" ) ;

        vb->resize( awire.positions.size() / nvc ) ;
        
        vb->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::position ) ;

        for( size_t i=0; i<awire.positions.size() / nvc; ++i )
        {
            vertex v = {
                so_math::vec3f_t( 
                    awire.positions[i*nvc+0], 
                    awire.positions[i*nvc+1],
                    awire.positions[i*nvc+2] )                
            } ;
            vb->set_vertex( i, v ) ;
        }

        ib->resize( awire.indices.size() ) ;
        for( size_t i=0; i<awire.indices.size(); ++i )
        {
            ib->set_index( i, awire.indices[i] ) ;
        }

        _vb = vb ;
        _ib = ib ;
    }
    else
    {
        so_log::log::warning( "[so_manager::buffer_creator::construct_from] : \
                               so_geo::flat_line_mesh_cref_t - unsupported position format" ) ;
    }
}