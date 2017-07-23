//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_HANDLER_HANDLER_H_
#define _SNAKEOIL_AUDIO_HANDLER_HANDLER_H_

#include "../typedefs.h"
#include "../buffer/pcm/pcm_buffer.hpp"


#include <functional>

namespace so_audio
{
    so_typedefs( std::function< void_t ( so_audio::ipcm_buffer_cref_t ) >, loopback_handler ) ;
}

#endif