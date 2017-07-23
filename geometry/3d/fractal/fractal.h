//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_3D_FRACTAL_H_
#define _SNAKEOIL_GEOMETRY_3D_FRACTAL_H_

#include "../../typedefs.h"
#include "../../api.h"

#include "fractal_object.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_geo
{
    namespace so_3d
    {
        struct SNAKEOIL_GEOMETRY_API fractal
        {
            so_this_typedefs( fractal ) ;

            so_typedefs( so_std::vector<fractal_object>, objects ) ;

            typedef std::function< objects_t ( fractal_object_cref_t ) > divider_funk_t ;
            

            static objects_t menger_sponge( fractal_object_cref_t ) ;
            static objects_t sierpinski_tetra( fractal_object_cref_t ) ;

            static divider_funk_t menger_sponge_funk( void_t ) ;
            static divider_funk_t sierpinski_tetra_funk( void_t ) ;

            static objects_t iterate( divider_funk_t, fractal_object_cref_t, size_t ) ;
        };
    }
}

#endif
