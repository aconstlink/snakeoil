//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_HANDLER_HANDLER_HANDLE_H_
#define _SNAKEOIL_AUDIO_HANDLER_HANDLER_HANDLE_H_

#include "../api.h"
#include "../protos.h"
#include "../typedefs.h"
#include "handler_id.h"

namespace so_audio
{
    struct SNAKEOIL_AUDIO_API handler_handle
    {
        so_audio::iaudio_engine_ptr_t ae_ptr ;
        so_audio::handler_id_t id ;
    };
    so_typedef( handler_handle ) ;
}

#endif