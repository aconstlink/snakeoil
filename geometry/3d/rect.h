//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_3D_RECT_H_
#define _SNAKEOIL_GEOMETRY_3D_RECT_H_

#include "../result.h"
#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"

#include <snakeoil/math/vector/vector3.hpp>

namespace so_geo
{
    namespace so_3d
    {
        struct SNAKEOIL_GEOMETRY_API rect
        {
            struct input_params
            {
                /// per dimension scale in object space!
                so_math::vec3f_t scale = so_math::vec3f_t( 1.0f );

                /// tessellation :
                /// vertices per side (vps) = tess + 1
                /// vertices per face (vpf) = vps * vps 
                /// vertices per cube (vpc) = vpf * 6 
                size_t tess = 1 ;
            };
            so_typedef( input_params ) ;

            static so_geo::result make( so_geo::flat_line_mesh_inout_t, input_params_cref_t ) ;
        };
        so_typedef( rect ) ;
    }
}

#endif
