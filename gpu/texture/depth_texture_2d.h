//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_TEXTURE_DEPTH_TEXTURE_2D_H_
#define _SNAKEOIL_GPU_TEXTURE_DEPTH_TEXTURE_2D_H_

#include "texture_2d.h"

namespace so_gpu
{
    class SNAKEOIL_GPU_API depth_texture_2d : public so_gpu::texture_2d
    {
        typedef so_gpu::texture_2d base_t ;
        so_this_typedefs( depth_texture_2d ) ;

    public:

        depth_texture_2d( iimage_2d_ptr_t ) ;
        depth_texture_2d( this_rref_t rhv ) ;
        virtual ~depth_texture_2d( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;
    };
    so_typedef( depth_texture_2d ) ;
}

#endif

