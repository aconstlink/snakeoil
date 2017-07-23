//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GFX_CAMERA_ICAMERA_H_
#define _SNAKEOIL_GFX_CAMERA_ICAMERA_H_

#include "../typedefs.h"
#include "../result.h"
#include "../api.h"
#include "../protos.h"

#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/utility/3d/transformation.hpp>

namespace so_gfx
{
    class SNAKEOIL_GFX_API icamera 
    {
    public:

        virtual size_t get_num_lenses( void_t ) const = 0 ;
        virtual ilens_ptr_t get_lens( size_t ) = 0 ;

    public:

        virtual void_t transform_by( so_math::so_3d::trafof_cref_t ) = 0 ;
        virtual void_t set_transformaion( so_math::so_3d::trafof_cref_t ) = 0 ;
        virtual void_t get_transformation( so_math::so_3d::trafof_ref_t ) const = 0 ;


        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
