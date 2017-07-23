//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_IINDEX_BUFFER_H_
#define _SNAKEOIL_GPU_BUFFER_IINDEX_BUFFER_H_

#include "buffer.h"

namespace so_gpu
{
    class iindex_buffer : public buffer
    {
    protected:

        iindex_buffer( void_t ) {}
        iindex_buffer( iindex_buffer && rhv ) : buffer( std::move(rhv) ) {}

    public:

        virtual ~iindex_buffer( void_t ) {}

    public:       

        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( iindex_buffer ) ;
}

#endif

