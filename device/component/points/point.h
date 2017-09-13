//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#if 0

#include "../comp.h"

#include <snakeoil/math/vector/vector2.hpp>

namespace so_device
{
    namespace so_component
    {
        struct point : public comp<point>
        {
            size_t cid = size_t( -1 ) ;

            /// in [-1,1]
            so_math::vec2f_t intensity = so_math::vec2f_t(0.0f) ;
        };
        comp_type const comp<point>::ct = comp_type::point ;

        so_typedef( point ) ;
    }
}

#endif
