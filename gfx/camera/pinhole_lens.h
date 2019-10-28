//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "ilens.h"

namespace so_gfx
{
    class SNAKEOIL_GFX_API pinhole_lens : public ilens
    {
        so_this_typedefs( pinhole_lens ) ;

    private:

        enum class projection_type
        {
            undefined,
            orthographic,
            perspective
        };

    private:
        
        so_math::mat3f_t _lens_frame ;
        so_math::mat4f_t _lens_matrix ;
        so_math::mat4f_t _view_matrix ;
        so_math::mat4f_t _proj_matrix ;

        projection_type _projection_mode ;

    public:

        pinhole_lens( void_t ) ;
        pinhole_lens( this_rref_t ) ;
        virtual ~pinhole_lens( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        this_ref_t make_orthographic( float_t const w, float_t const h,
            float_t const near, float_t const far ) noexcept ;

        this_ref_t make_perspective_fov( float_t const fov, float_t const aspect,
            float_t const near, float_t const far ) noexcept ;

        static this_t create_orthographic( float_t const w, float_t const h, 
            float_t const near, float_t const far ) noexcept ;
        static this_t create_perspective_fov( float_t const fov, float_t const aspect, 
            float_t const near, float_t const far ) noexcept ;
        
        bool_t is_perspective( void_t ) const ;
        bool_t is_orthographic( void_t ) const ;
        
    public: // interface

        virtual void_t get_position( so_math::vec3f_ref_t ) const ;
     
        virtual void_t get_lens_frame( so_math::mat3f_ref_t ) const ;
        virtual void_t get_lens_matrix( so_math::mat4f_ref_t ) const ;
        virtual void_t get_view_matrix( so_math::mat4f_ref_t ) const ;
        virtual void_t get_proj_matrix( so_math::mat4f_ref_t ) const ;
        
        virtual void_t transform_by( so_math::so_3d::trafof_cref_t ) ;
        virtual void_t set_transformation( so_math::so_3d::trafof_cref_t ) ;
        virtual void_t look_at( so_math::vec3f_cref_t pos, 
            so_math::vec3f_cref_t up, so_math::vec3f_cref_t at ) ;

        void_t set_camera_matrix( so_math::mat4f_cref_t ) ;
        
    public:

        virtual void_t destroy( void_t ) ;

    };
    so_typedef( pinhole_lens ) ;
}