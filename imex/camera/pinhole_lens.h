//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_CAMERA_PINHOLE_LENS_H_
#define _SNAKEOIL_IMEX_CAMERA_PINHOLE_LENS_H_

#include "ilens.h"

namespace so_imex
{
    class SNAKEOIL_IMEX_API pinhole_lens : public ilens
    {
        so_this_typedefs( pinhole_lens ) ;

    private:

        // in radian
        float_t _fov_horz = 0.0f ;
        float_t _fov_vert = 0.0f ;
        float_t _aspect = 0.0f ;
        float_t _near = 0.0f ;
        float_t _far = 1.0f ;

    public:

        pinhole_lens( void_t ) ;
        pinhole_lens( this_rref_t ) ;
        virtual ~pinhole_lens( void_t ) ;

    public:
        
        void_t set_far( float_t ) ;
        float_t get_far( void_t ) const ;

        void_t set_near( float_t ) ;
        float_t get_near( void_t ) const ;

        void_t set_aspect( float_t ) ;
        float_t get_aspect( void_t ) const ;

        /// horizontal and vertical fov in radian
        void_t set_fov_hv( float_t h, float_t v ) ;
        void_t set_fovh_aspect( float_t h, float_t a ) ;
        void_t set_fovv_aspect( float_t v, float_t a ) ;

        float_t get_fovh( void_t ) const ;
        float_t get_fovv( void_t ) const ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:
        
        virtual void_t destroy( void_t ) ;
    };

    so_typedef( pinhole_lens ) ;
}

#endif
