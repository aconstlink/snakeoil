//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_GEOMETRY_H_
#define _SNAKEOIL_GPU_BUFFER_GEOMETRY_H_

#include "ivertex_buffer.h"
#include "iindex_buffer.h"
#include "../enums/primitive_types.h"

namespace so_gpu
{
    class geometry
    {
        so_this_typedefs( geometry ) ;

    private:

        so_gpu::primitive_type _pt = so_gpu::primitive_type::undefined ;
        so_gpu::ivertex_buffer_ptr_t _vb_ptr = nullptr ;
        so_gpu::iindex_buffer_ptr_t _ib_ptr = nullptr ;

    public:

        geometry( void_t ) ;
        geometry( this_cref_t ) = delete ;
        geometry( this_rref_t ) ;
        virtual ~geometry( void_t ) ;

    };
}

#endif