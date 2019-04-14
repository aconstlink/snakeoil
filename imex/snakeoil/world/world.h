
#pragma once

#include "../../api.h"
#include "../../typedefs.h"


#include <snakeoil/io/typedefs.h>
#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector4.hpp>

namespace so_imex
{
    namespace so_snakeoil
    {
        struct material
        {
            size_t id ;
        };
        so_typedef( material ) ;

        struct block
        {
            material_t mat ;
        };
        so_typedef( block ) ;

        struct chunk
        {

        };

        struct world
        {
            so_typedefs( so_std::vector< material_t >, materials ) ;
            materials_t materials ;
        };
        so_typedef( world ) ;
    }
}