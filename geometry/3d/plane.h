//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_3D_PLANE_H_
#define _SNAKEOIL_GEOMETRY_3D_PLANE_H_

#include "../result.h"
#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>

namespace so_geo
{
    namespace so_3d
    {
        struct SNAKEOIL_GEOMETRY_API plane
        {
            so_this_typedefs( plane ) ;

        private:

            struct data
            {
                floats_t positions ;
                floats_t normals ;
                more_floats_t texcoords ;

                /// quads per dim
                size_t qpd = 0 ;
                /// vertices per dim
                size_t vpd = 0 ;
                /// num vertices
                size_t nv = 0 ;
            };
            so_typedef( data ) ;

        public:

            struct input_params
            {
                /// per dimension scale in object space!
                so_math::vec2f_t scale = so_math::vec2f_t( 1.0f );
                /// per dimension rotation - euler angles in object space!
                so_math::vec3f_t euler = so_math::vec3f_t( 0.0f );

                /// number of quads per side
                /// => vertices per side = tess + 1 
                size_t tess = 1 ;
            };
            so_typedef( input_params ) ;

            static so_geo::result make( polygon_mesh_ptr_t, input_params_cref_t ) ;
            
        private:

            static void_t construct_vertices( data_ref_t, input_params_cref_t ) ;
            
        };
        so_typedef( plane ) ;
    }
}

#endif
