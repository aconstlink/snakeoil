//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "buffer_creator.h"

#include <snakeoil/gpu/enums/vertex_attributes.h>
#include <snakeoil/gpu/buffer/vertex_buffer.hpp>
#include <snakeoil/gpu/buffer/index_buffer.hpp>
#include <snakeoil/gpu/program/config.h>

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>

using namespace so_manager ;
using namespace so_manager::so_graphics ;

//*************************************************************************************
void_t buffer_creator::construct_from( so_geo::flat_tri_mesh_cref_t amesh ) 
{
    _pt = so_gpu::primitive_type::triangles ;

    bool_t const has_normals = amesh.normals.size() > 0 ;
    size_t const num_uv_layers = amesh.texcoords.size() ;

    if( has_normals && num_uv_layers > 0 )
    {
        if( num_uv_layers == 1 )
        {
            if( amesh.position_format == so_geo::vector_component_format::xy &&
                amesh.normal_format == so_geo::vector_component_format::xy )
            {
                struct vertex 
                {
                    so_math::vec2f_t pos ;
                    so_math::vec2f_t nrm ;
                    so_math::vec2f_t uv0 ;
                };
                so_typedefs( so_gpu::vertex_buffer<vertex>, vb ) ;
                so_typedefs( so_gpu::index_buffer<uint_t>, ib ) ;

                 vb_ptr_t vb = vb_t::create( 
                    "[so_manager::mesh::construct_from] : vertex buffer" ) ;
                 ib_ptr_t ib = ib_t::create(
                    "[so_manager::mesh::construct_from] : index buffer" ) ;

                vb->resize( amesh.positions.size() / 2 ) ;
                ib->resize( amesh.indices.size() ) ;

                vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::position ) ;
                vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::normal ) ;
                vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::texcoord0 ) ;

                for( size_t i=0; i<amesh.positions.size() / 2; ++i )
                {
                    vertex v = {
                        so_math::vec2f_t(amesh.positions[i*2+0], amesh.positions[i*2+1]),
                        so_math::vec2f_t(amesh.normals[i*2+0], amesh.normals[i*2+1]),
                        so_math::vec2f_t(amesh.texcoords[0][i*2+0], amesh.texcoords[0][i*2+1])
                    } ;
                    vb->set_vertex( i, v ) ;
                }
                for( size_t i=0; i<amesh.indices.size(); ++i )
                {
                    ib->set_index( i, amesh.indices[i] ) ;
                }

                _vb = vb ;
                _ib = ib ;
            }
            else if( 
                amesh.position_format == so_geo::vector_component_format::xyz &&
                amesh.normal_format == so_geo::vector_component_format::xyz &&
                amesh.texcoord_format == so_geo::texcoord_component_format::uv )
            {

                struct vertex 
                {
                    so_math::vec3f_t pos ;
                    so_math::vec3f_t nrm ;
                    so_math::vec2f_t uv0 ;
                };
                so_typedefs( so_gpu::vertex_buffer<vertex>, vb ) ;
                so_typedefs( so_gpu::index_buffer<uint_t>, ib ) ;

                 vb_ptr_t vb = vb_t::create( 
                    "[so_manager::mesh::construct_from] : vertex buffer" ) ;
                 ib_ptr_t ib = ib_t::create(
                    "[so_manager::mesh::construct_from] : index buffer" ) ;

                vb->resize( amesh.positions.size() / 3 ) ;
                ib->resize( amesh.indices.size() ) ;

                vb->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::position ) ;
                vb->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::normal ) ;
                vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::texcoord0 ) ;

                for( size_t i=0; i<amesh.positions.size() / 3; ++i )
                {
                    vertex v = {
                        so_math::vec3f_t(
                            amesh.positions[i*3+0], amesh.positions[i*3+1], amesh.positions[i*3+2]),
                        so_math::vec3f_t(
                            amesh.normals[i*3+0], amesh.normals[i*3+1], amesh.normals[i*3+2]),
                        so_math::vec2f_t(
                            amesh.texcoords[0][i*2+0], amesh.texcoords[0][i*2+1] )
                    } ;
                    vb->set_vertex( i, v ) ;
                }
                for( size_t i=0; i<amesh.indices.size(); ++i )
                {
                    ib->set_index( i, amesh.indices[i] ) ;
                }

                _vb = vb ;
                _ib = ib ;
            }
        }
        else if( num_uv_layers == 2 )
        {
            if( amesh.position_format == so_geo::vector_component_format::xy &&
                amesh.normal_format == so_geo::vector_component_format::xy &&
                amesh.texcoord_format == so_geo::texcoord_component_format::uv )
            {
                struct vertex 
                {
                    so_math::vec2f_t pos ;
                    so_math::vec2f_t nrm ;
                    so_math::vec2f_t uv0 ;
                    so_math::vec2f_t uv1 ;
                };
                so_typedefs( so_gpu::vertex_buffer<vertex>, vb ) ;
                so_typedefs( so_gpu::index_buffer<uint_t>, ib ) ;

                 vb_ptr_t vb = vb_t::create( 
                    "[so_manager::mesh::construct_from] : vertex buffer" ) ;
                 ib_ptr_t ib = ib_t::create(
                    "[so_manager::mesh::construct_from] : index buffer" ) ;

                vb->resize( amesh.positions.size() / 2 ) ;
                ib->resize( amesh.indices.size() ) ;

                vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::position ) ;
                vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::normal ) ;
                vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::texcoord0 ) ;
                vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::texcoord1 ) ;

                for( size_t i=0; i<amesh.positions.size() / 2; ++i )
                {
                    vertex v = {
                        so_math::vec2f_t(amesh.positions[i*2+0], amesh.positions[i*2+1]),
                        so_math::vec2f_t(amesh.normals[i*2+0], amesh.normals[i*2+0]),
                        so_math::vec2f_t(amesh.texcoords[0][i*2+0], amesh.texcoords[0][i*2+0]),
                        so_math::vec2f_t(amesh.texcoords[1][i*2+0], amesh.texcoords[1][i*2+0])
                    } ;
                    vb->set_vertex( i, v ) ;
                }
                for( size_t i=0; i<amesh.indices.size(); ++i )
                {
                    ib->set_index( i, amesh.indices[i] ) ;
                }

                _vb = vb ;
                _ib = ib ;
            }
            else if( 
                amesh.position_format == so_geo::vector_component_format::xyz &&
                amesh.normal_format == so_geo::vector_component_format::xyz &&
                amesh.texcoord_format == so_geo::texcoord_component_format::uv )
            {

                struct vertex 
                {
                    so_math::vec3f_t pos ;
                    so_math::vec3f_t nrm ;
                    so_math::vec2f_t uv0 ;
                    so_math::vec2f_t uv1 ;
                };
                so_typedefs( so_gpu::vertex_buffer<vertex>, vb ) ;
                so_typedefs( so_gpu::index_buffer<uint_t>, ib ) ;

                 vb_ptr_t vb = vb_t::create( 
                    "[so_manager::mesh::construct_from] : vertex buffer" ) ;
                 ib_ptr_t ib = ib_t::create(
                    "[so_manager::mesh::construct_from] : index buffer" ) ;

                vb->resize( amesh.positions.size() / 3 ) ;
                ib->resize( amesh.indices.size() ) ;

                vb->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::position ) ;
                vb->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::normal ) ;
                vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::texcoord0 ) ;
                vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::texcoord1 ) ;

                for( size_t i=0; i<amesh.positions.size() / 3; ++i )
                {
                    vertex v = {
                        so_math::vec3f_t(
                            amesh.positions[i*3+0], amesh.positions[i*3+1], amesh.positions[i*3+2]),
                        so_math::vec3f_t(
                            amesh.normals[i*3+0], amesh.normals[i*3+1], amesh.normals[i*3+2]),
                        so_math::vec2f_t(
                            amesh.texcoords[0][i*2+0], amesh.texcoords[0][i*2+1] ),
                        so_math::vec2f_t(
                            amesh.texcoords[1][i*2+0], amesh.texcoords[1][i*2+1] )
                    } ;
                    vb->set_vertex( i, v ) ;
                }
                for( size_t i=0; i<amesh.indices.size(); ++i )
                {
                    ib->set_index( i, amesh.indices[i] ) ;
                }

                _vb = vb ;
                _ib = ib ;
            }
        }
        
    }
    else if( has_normals )
    {
        if( amesh.position_format == so_geo::vector_component_format::xyz &&
            amesh.normal_format == so_geo::vector_component_format::xyz )
            {
                struct vertex 
                {
                    so_math::vec3f_t pos ;
                    so_math::vec3f_t nrm ;
                };
                so_typedefs( so_gpu::vertex_buffer<vertex>, vb ) ;
                so_typedefs( so_gpu::index_buffer<uint_t>, ib ) ;

                 vb_ptr_t vb = vb_t::create( 
                    "[so_manager::mesh::construct_from] : vertex buffer" ) ;
                 ib_ptr_t ib = ib_t::create(
                    "[so_manager::mesh::construct_from] : index buffer" ) ;

                vb->resize( amesh.positions.size() / 3) ;
                ib->resize( amesh.indices.size() ) ;

                vb->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::position ) ;
                vb->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::normal ) ;

                for( size_t i=0; i<amesh.positions.size() / 3; ++i )
                {
                    vertex v = {
                        so_math::vec3f_t(
                            amesh.positions[i*3+0], amesh.positions[i*3+1], amesh.positions[i*3+2]),
                        so_math::vec3f_t(
                            amesh.normals[i*3+0], amesh.normals[i*3+1], amesh.normals[i*3+2])
                    } ;
                    vb->set_vertex( i, v ) ;
                }
                for( size_t i=0; i<amesh.indices.size(); ++i )
                {
                    ib->set_index( i, amesh.indices[i] ) ;
                }

                _vb = vb ;
                _ib = ib ;
            }
    }
    // positions/texcoords, no normals
    else if( num_uv_layers == 1 )
    {
        if( amesh.position_format == so_geo::vector_component_format::xyz &&
            amesh.texcoord_format == so_geo::texcoord_component_format::uv )
        {
            struct vertex
            {
                so_math::vec3f_t pos ;
                so_math::vec2f_t tx0 ;
            };
            so_typedefs( so_gpu::vertex_buffer<vertex>, vb ) ;
            so_typedefs( so_gpu::index_buffer<uint_t>, ib ) ;

            vb_ptr_t vb = vb_t::create( "[so_manager::mesh::construct_from] : vertex buffer" ) ;
            ib_ptr_t ib = ib_t::create( "[so_manager::mesh::construct_from] : index buffer" ) ;
           
            vb->resize( amesh.positions.size() / 3 ) ;
            ib->resize( amesh.indices.size() ) ;

            vb->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::position ) ;
            vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::texcoord0 ) ;

            for( size_t i = 0; i < amesh.positions.size() / 3; ++i )
            {
                vertex v = {
                    so_math::vec3f_t(
                        amesh.positions[i * 3 + 0], amesh.positions[i * 3 + 1], amesh.positions[i * 3 + 2] ),
                    so_math::vec2f_t(
                        amesh.texcoords[0][i * 2 + 0], amesh.texcoords[0][i * 2 + 1] )
                } ;
                vb->set_vertex( i, v ) ;
            }

            for( size_t i = 0; i < amesh.indices.size(); ++i )
            {
                ib->set_index( i, amesh.indices[i] ) ;
            }

            _vb = vb ;
            _ib = ib ;
        }
        else
        {
            so_log::log::warning( "[so_manager::mesh::construct_from] : unsupported format") ;
        }
        
    }
    else if( num_uv_layers == 2 )
    {
        if( amesh.position_format == so_geo::vector_component_format::xyz &&
            amesh.texcoord_format == so_geo::texcoord_component_format::uv )
        {
            struct vertex
            {
                so_math::vec3f_t pos ;
                so_math::vec2f_t tx0 ;
                so_math::vec2f_t tx1 ;
            };
            so_typedefs( so_gpu::vertex_buffer<vertex>, vb ) ;
            so_typedefs( so_gpu::index_buffer<uint_t>, ib ) ;

            vb_ptr_t vb = vb_t::create( "[so_manager::mesh::construct_from] : vertex buffer" ) ;
            ib_ptr_t ib = ib_t::create( "[so_manager::mesh::construct_from] : index buffer" ) ;

            vb->resize( amesh.positions.size() / 3 ) ;
            ib->resize( amesh.indices.size() ) ;

            vb->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::position ) ;
            vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::texcoord0 ) ;
            vb->add_layout_element<so_math::vec2f_t>( so_gpu::vertex_attribute::texcoord1 ) ;

            for( size_t i = 0; i < amesh.positions.size() / 3; ++i )
            {
                vertex v = {
                    so_math::vec3f_t(
                        amesh.positions[i * 3 + 0], amesh.positions[i * 3 + 1], amesh.positions[i * 3 + 2] ),
                    so_math::vec2f_t(
                        amesh.texcoords[0][i * 2 + 0], amesh.texcoords[0][i * 2 + 1] ),
                    so_math::vec2f_t(
                        amesh.texcoords[1][i * 2 + 0], amesh.texcoords[1][i * 2 + 1] )
                } ;
                vb->set_vertex( i, v ) ;
            }

            for( size_t i = 0; i < amesh.indices.size(); ++i )
            {
                ib->set_index( i, amesh.indices[i] ) ;
            }

            _vb = vb ;
            _ib = ib ;
        }
        else
        {
            so_log::log::warning( "[so_manager::mesh::construct_from] : unsupported format" ) ;
        }

    }
    // positions only
    // no normals/ no texcoords
    else
    {
        if( amesh.position_format == so_geo::vector_component_format::xyz )
        {
            struct vertex
            {
                so_math::vec3f_t pos ;
            };
            so_typedefs( so_gpu::vertex_buffer<vertex>, vb ) ;
            so_typedefs( so_gpu::index_buffer<uint_t>, ib ) ;

            vb_ptr_t vb = vb_t::create( "[so_manager::mesh::construct_from] : vertex buffer" ) ;
            ib_ptr_t ib = ib_t::create( "[so_manager::mesh::construct_from] : index buffer" ) ;

            vb->resize( amesh.positions.size() / 3 ) ;
            ib->resize( amesh.indices.size() ) ;

            vb->add_layout_element<so_math::vec3f_t>( so_gpu::vertex_attribute::position ) ;

            for(size_t i = 0; i < amesh.positions.size() / 3; ++i)
            {
                vertex v = {
                    so_math::vec3f_t(
                    amesh.positions[i * 3 + 0], amesh.positions[i * 3 + 1], amesh.positions[i * 3 + 2] )
                } ;
                vb->set_vertex( i, v ) ;
            }
            for(size_t i = 0; i < amesh.indices.size(); ++i)
            {
                ib->set_index( i, amesh.indices[i] ) ;
            }

            _vb = vb ;
            _ib = ib ;
        }
    }
}