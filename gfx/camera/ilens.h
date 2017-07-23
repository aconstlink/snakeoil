//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GFX_CAMERA_ILENS_H_
#define _SNAKEOIL_GFX_CAMERA_ILENS_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"

#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/matrix/matrix3.hpp>
#include <snakeoil/math/matrix/matrix4.hpp>
#include <snakeoil/math/utility/3d/transformation.hpp>

namespace so_gfx
{
    class SNAKEOIL_GFX_API ilens
    {
    public:

        /// lens position relative to camera
        virtual void_t get_position( so_math::vec3f_ref_t ) const = 0 ;
        
        virtual void_t get_lens_frame( so_math::mat3f_ref_t ) const = 0 ;
        virtual void_t get_lens_matrix( so_math::mat4f_ref_t ) const = 0 ;
        virtual void_t get_view_matrix( so_math::mat4f_ref_t ) const = 0 ;
        virtual void_t get_proj_matrix( so_math::mat4f_ref_t ) const = 0 ;

        virtual void_t transform_by( so_math::so_3d::trafof_cref_t ) = 0 ;
        virtual void_t set_transformation( so_math::so_3d::trafof_cref_t ) = 0 ;

        virtual void_t look_at( so_math::vec3f_cref_t pos, 
            so_math::vec3f_cref_t up, so_math::vec3f_cref_t at ) = 0 ;

    public:

        virtual void_t destroy( void_t ) = 0 ;

    };
}

#endif
