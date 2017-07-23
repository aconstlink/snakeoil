//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_IMAGE_IIMAGE_2D_H_
#define _SNAKEOIL_GPU_IMAGE_IIMAGE_2D_H_

#include "image.h"

namespace so_gpu
{
    class SNAKEOIL_GPU_API iimage_2d : public image
    {

    public:

        iimage_2d( void_t ) {} 
        iimage_2d( iimage_2d && rhv ) : image( std::move( rhv ) ) {}
        virtual ~iimage_2d( void_t ) {}

    public:

        virtual size_t get_width( void_t ) const = 0 ;
        virtual size_t get_height( void_t ) const = 0 ;
        virtual void_t resize( size_t width, size_t height ) = 0 ;

    };
    so_typedef( iimage_2d ) ;
}

#endif

