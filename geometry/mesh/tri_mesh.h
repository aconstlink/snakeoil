//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_MESH_TRI_MESH_H_
#define _SNAKEOIL_GEOMETRY_MESH_TRI_MESH_H_

#include "imesh.h"

#include "flat_tri_mesh.h"

namespace so_geo
{
    /// a mesh with triangles. Every three indices 
    /// form a triangle.
    /// @note this mesh type does not have a polygon
    /// indirection table. The polygon's(tri's) vertices
    /// are indirected by the index tables. Every three
    /// indices make a triangle.
    struct SNAKEOIL_GEOMETRY_API tri_mesh : public imesh
    {
        so_this_typedefs( tri_mesh ) ;

    public:

        /// defines triangles => 
        /// every 3 indices represent a triangle
        uints_t indices ;
        more_uints_t texcoords_indices ;
        more_uints_t normals_indices ;

        /// x,y,z, x,y,z, ...
        floats_t positions ;
        more_floats_t texcoords ;
        more_floats_t normals ;

        vector_component_format position_format ;
        vector_component_format normal_format ;
        texcoord_component_format texcoord_format ;

    public:

        tri_mesh( void_t ) ;
        tri_mesh( this_cref_t ) ;
        tri_mesh( this_rref_t ) ;
        virtual ~tri_mesh( void_t ) ;

        this_ref_t operator = ( this_cref_t ) ;
        this_ref_t operator = ( this_rref_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_cref_t, so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_geo::result flatten( flat_tri_mesh_ref_t mesh_out ) const ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( tri_mesh ) ;
    
}

#endif
