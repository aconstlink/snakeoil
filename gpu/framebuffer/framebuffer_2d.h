//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_FRAMEBUFFER_FRAMEBUFFER_2D_H_
#define _SNAKEOIL_GEOMETRY_FRAMEBUFFER_FRAMEBUFFER_2D_H_

#include "framebuffer.h"

namespace so_gpu
{
    class SNAKEOIL_GPU_API framebuffer_2d : public so_gpu::framebuffer
    {
        so_this_typedefs( framebuffer_2d ) ;
        
        typedef so_std::vector< texture_2d_ptr_t > color_targets_t ;
        
    private:

        color_targets_t _colors ;
        depth_texture_2d_ptr_t _depth = nullptr ;

    private:

        framebuffer_2d( this_cref_t ) {}

    public:

        framebuffer_2d( void_t ) ;
        framebuffer_2d( this_rref_t ) ;
        virtual ~framebuffer_2d( void_t ) ;
    
    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t add_color_target( texture_2d_ptr_t ) ;
        void_t set_depth_target( depth_texture_2d_ptr_t ) ;

    public:

        size_t get_num_color_targets( void_t ) const ;
        texture_2d_ptr_t get_color_target( size_t ) ;
        depth_texture_2d_ptr_t get_depth_target( void_t ) ;

        color_targets_t & get_color_targets( void_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( framebuffer_2d ) ;
}

#endif

