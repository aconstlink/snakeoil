//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GFX_CAMERA_GENERIC_CAMERA_H_
#define _SNAKEOIL_GFX_CAMERA_GENERIC_CAMERA_H_

#include "icamera.h"

#include <snakeoil/math/utility/3d/transformation.hpp>

#include <vector>

namespace so_gfx
{
    class SNAKEOIL_GFX_API generic_camera : public icamera
    {
        so_this_typedefs( generic_camera ) ;
        so_typedefs( std::vector<ilens_ptr_t>, lenses ) ;

    private:

        so_math::so_3d::trafof_t _trafo ;
        lenses_t _lenses ;

    public: // 
         
        generic_camera( void_t ) noexcept ;
        generic_camera( so_gfx::ilens_utr_t ) noexcept ;
        generic_camera( this_rref_t ) noexcept ;
        generic_camera( this_cref_t ) = delete ;
        virtual ~generic_camera( void_t ) noexcept ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_gfx::result add_lens( so_gfx::ilens_utr_t ) ;

    public: // interface

        virtual size_t get_num_lenses( void_t ) const ;
        virtual ilens_ptr_t get_lens( size_t ) ;

        virtual void_t transform_by( so_math::so_3d::trafof_cref_t ) ;
        virtual void_t set_transformaion( so_math::so_3d::trafof_cref_t ) ;
        virtual void_t get_transformation( so_math::so_3d::trafof_ref_t ) const ;

    public:

        virtual void_t destroy( void_t ) ;

    };
    so_typedef( generic_camera ) ;
}

#endif
