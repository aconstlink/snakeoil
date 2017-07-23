//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_TEXTURE_TEXTURE_H_
#define _SNAKEOIL_GPU_TEXTURE_TEXTURE_H_

#include "../object.h"

namespace so_gpu
{
    class SNAKEOIL_GPU_API texture : public so_gpu::object
    {
        typedef so_gpu::object base_t ;

    private:

        bool_t _has_changed = true ;

    public:

        texture( void_t ) ;
        texture( texture && rhv ) ;
        virtual ~texture( void_t ) ;

    public:

        bool_t has_state_changed( void_t ) const ;
        void_t set_state_changed( bool_t b ) ;

    public:

        virtual so_gpu::image_ptr_t get_image( void_t ) = 0 ;
        virtual so_gpu::image_cptr_t get_image( void_t ) const = 0 ;
    };
    so_typedef( texture ) ;
}

#endif

