//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_CAMERA_GENERIC_CAMERA_H_
#define _SNAKEOIL_IMEX_CAMERA_GENERIC_CAMERA_H_

#include "icamera.h"

#include <snakeoil/math/vector/vector3.hpp>

#include <vector>

namespace so_imex
{
    class SNAKEOIL_IMEX_API generic_camera : public icamera
    {
        so_this_typedefs( generic_camera ) ;
        so_typedefs( std::vector<ilens_ptr_t>, lenses ) ;

    private:

        lenses_t _lenses ;

        so_math::vec3f_t _position = so_math::vec3f_t(0.0f) ;
        so_math::vec3f_t _up = so_math::vec3f_t(0.0f,1.0f,0.0f) ;
        so_math::vec3f_t _look = so_math::vec3f_t(0.0f,0.0f,1.0f) ;
        so_math::vec3f_t _right = so_math::vec3f_t(1.0f,0.0f,0.0f) ;
        
        // @todo angles and stuff

    public:

        generic_camera( void_t ) ;
        generic_camera( this_rref_t ) ;
        virtual ~generic_camera( void_t ) ;

    public:
        
        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t set_position( so_math::vec3f_cref_t ) ;
        so_math::vec3f_cref_t get_position( void_t ) const ;

        void_t set_up_vector( so_math::vec3f_cref_t ) ;
        so_math::vec3f_cref_t get_up_vector( void_t ) const ;

        void_t set_look( so_math::vec3f_cref_t ) ;
        so_math::vec3f_cref_t get_look( void_t ) const ;

        void_t set_right( so_math::vec3f_cref_t ) ;
        so_math::vec3f_cref_t get_right( void_t ) const ;

    public:

        size_t get_num_lenses( void_t ) const ;
        ilens_cptr_t get_lens( size_t i ) const ;

        so_imex::result add_lens( ilens_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( generic_camera ) ;
}

#endif
