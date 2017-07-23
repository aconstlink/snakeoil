//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_OBJ_OBJ_DOCUMENT_H_
#define _SNAKEOIL_IMEX_MODULE_OBJ_OBJ_DOCUMENT_H_

#include "../../typedefs.h"

#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/math/vector/vector3.hpp>

namespace so_imex
{
    namespace so_obj
    {
        struct obj_document
        {
            // vertex positions
            struct vertex_position
            {
                so_math::vec3f_t v ;
            };
            so_typedef( vertex_position ) ;
            so_typedefs( so_std::vector< vertex_position_t >, positions ) ;
            
            // vertex normals
            struct vertex_normal
            {
                so_math::vec3f_t v ;
            };
            so_typedef( vertex_normal ) ;
            so_typedefs( so_std::vector< vertex_normal_t>, normals ) ;

            // vertex texcoords
            struct vertex_texcoord
            {
                so_math::vec3f_t v ;
            };
            so_typedef( vertex_texcoord ) ;
            so_typedefs( so_std::vector< vertex_texcoord_t >, texcoords ) ;

            // face indices
            struct face
            {
                struct vertex
                {
                    size_t v = size_t( -1 ) ;
                    size_t vt = size_t( -1 ) ;
                    size_t vn = size_t( -1 ) ;
                };

                byte_t num_vertices = 0 ;
                vertex vertices[ 4 ] ;
            } ;
            so_typedef( face ) ;
            so_typedefs( so_std::vector< face_t >, faces ) ;


        public:

            so_std::string_t name = "default" ;
            positions_t positions ;
            normals_t normals ;
            texcoords_t texcoords ;
            faces_t faces ;
        };
        so_typedef( obj_document ) ;
    }
    
}

#endif