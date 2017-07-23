//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_3D_TETRA_H_
#define _SNAKEOIL_GEOMETRY_3D_TETRA_H_

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
        struct SNAKEOIL_GEOMETRY_API tetra
        {
            struct input_params
            {
                /// per dimension scale in object space!
                so_math::vec3f_t scale = so_math::vec3f_t( 1.0f );
                /// per dimension rotation - euler angles in object space!
                so_math::vec3f_t euler = so_math::vec3f_t( 0.0f );
            };
            so_typedef( input_params ) ;

            static so_geo::result make( polygon_mesh_ptr_t, input_params_cref_t ) ;
            
        };
    }
}

#endif
