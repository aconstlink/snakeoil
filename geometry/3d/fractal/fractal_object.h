//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_3D_FRACTAL_FRACTAL_OBJECT_H_
#define _SNAKEOIL_GEOMETRY_3D_FRACTAL_FRACTAL_OBJECT_H_

#include <snakeoil/math/vector/vector3.hpp>

namespace so_geo
{
    namespace so_3d
    {
        struct fractal_object
        {
            so_math::vec3f_t pos ;
            float_t side_width ;
        };
        so_typedef( fractal_object ) ;
    }
}

#endif
