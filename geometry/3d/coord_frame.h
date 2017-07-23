//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_3D_COORD_FRAME_H_
#define _SNAKEOIL_GEOMETRY_3D_COORD_FRAME_H_

#include "../result.h"
#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"

#include <snakeoil/math/vector/vector3.hpp>

namespace so_geo
{
    namespace so_3d
    {
        class SNAKEOIL_GEOMETRY_API coord_frame
        {
        public:

            struct input_params
            {
                so_math::vec3f_t scale = so_math::vec3f_t(1.0f) ;
            };
            so_typedef( input_params ) ;

        public:

            static so_geo::result make( flat_line_mesh_ptr_t, input_params_cref_t ) ;
        };
        so_typedef( coord_frame ) ;
    }
}

#endif
