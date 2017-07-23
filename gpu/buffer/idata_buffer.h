//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_IDATA_BUFFER_H_
#define _SNAKEOIL_GPU_BUFFER_IDATA_BUFFER_H_

#include "buffer.h"
#include "../enums/data_buffer_formats.h"

namespace so_gpu
{
    class idata_buffer : public buffer
    {
    private:

        data_buffer_format _dbf = data_buffer_format::undefined ;

    protected:

        idata_buffer( void_t ) {}
        idata_buffer( idata_buffer && rhv ) : buffer( std::move(rhv) ) 
        {
            _dbf = rhv._dbf ;
        }

    public:

        virtual ~idata_buffer( void_t ) {}

        so_gpu::data_buffer_format get_data_buffer_format( void_t ) const { return _dbf ; }
        void_t set_data_buffer_format( so_gpu::data_buffer_format dbf ) { _dbf = dbf ; }

        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( idata_buffer ) ;
}

#endif

