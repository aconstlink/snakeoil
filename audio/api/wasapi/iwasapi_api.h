//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_API_IWASAPI_API_H_
#define _SNAKEOIL_AUDIO_API_IWASAPI_API_H_

#include "../iapi.h"
#include "../../handler/handler.h"
#include "../../handler/handler_id.h"

namespace so_audio
{
    class SNAKEOIL_AUDIO_API iwasapi_api : public iapi
    {
    public:

        virtual so_audio::result create( loopback_buffer_ptr_t ) = 0 ;
        virtual so_audio::result release( loopback_buffer_ptr_t ) = 0 ;
        virtual bool_t install_handler( so_audio::loopback_handler_t, handler_id_out_t ) = 0 ;

    public:

        virtual so_audio::result store( loopback_buffer_ptr_t ) = 0 ;
    };
}

#endif