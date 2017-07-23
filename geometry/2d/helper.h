//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_2D_HELPER_H_
#define _SNAKEOIL_GEOMETRY_2D_HELPER_H_

#include "../typedefs.h"

#include <snakeoil/math/vector/vector2.hpp>

namespace so_geo
{
    namespace so_2d
    {
        struct helper
        {        
            /// @param index the vector index
            static void_t vector_to_array( size_t index, so_math::vec2f_cref_t in_vec, floats_ref_t to ) 
            {
                to[index+0] = in_vec.x() ;
                to[index+1] = in_vec.y() ;
            }
        };
    }
}

#endif
