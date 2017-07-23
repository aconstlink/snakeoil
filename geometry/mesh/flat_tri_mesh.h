//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_MESH_FLAT_TRI_MESH_H_
#define _SNAKEOIL_GEOMETRY_MESH_FLAT_TRI_MESH_H_

#include "../typedefs.h"
#include "../api.h"
#include "../result.h"

#include "imesh.h"

#include "../enums/component_formats.h"

namespace so_geo
{
    /// this mesh type only has one indirection table.
    /// that indirection table(index buffer) connects
    /// the mesh's vertices to triangles.
    struct SNAKEOIL_GEOMETRY_API flat_tri_mesh : public imesh
    {
        so_this_typedefs( flat_tri_mesh ) ;

    public:

        /// defines triangles => 
        /// every 3 indices represent a triangle
        uints_t indices ;
        floats_t positions ;
        floats_t normals ;
        more_floats_t texcoords ;
        
        vector_component_format position_format ;
        vector_component_format normal_format ;
        texcoord_component_format texcoord_format ;

    public:

        flat_tri_mesh( void_t ) ;
        flat_tri_mesh( this_cref_t ) ;
        flat_tri_mesh( this_rref_t ) ;
        virtual ~flat_tri_mesh( void_t ) ;

    public:

        this_ref_t operator = ( this_rref_t ) ;
        this_ref_t operator = ( this_cref_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_cref_t, so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        bool_t are_array_numbers_valid( void_t ) const ;

        size_t get_num_vertices( void_t ) const ;

        size_t get_num_positions( void_t ) const ;
        size_t get_num_normals( void_t ) const ;
        size_t get_num_texcoords( size_t const ) const ;

    public:

        virtual void_t destroy( void_t ) ;

    };
    so_typedef( flat_tri_mesh ) ;
}

#endif
