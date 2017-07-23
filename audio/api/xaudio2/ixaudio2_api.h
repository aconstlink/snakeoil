//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_API_IXAUDIO2_API_H_
#define _SNAKEOIL_AUDIO_API_IXAUDIO2_API_H_

#include "../iapi.h"
#include "pcm_info.h"

namespace so_audio
{
    class SNAKEOIL_AUDIO_API ixaudio2_api : public iapi
    {
    public:

        virtual so_audio::result create( so_audio::audio_buffer_ptr_t, so_xaudio2::pcm_info_cref_t ) = 0 ;
        virtual so_audio::result release( so_audio::audio_buffer_ptr_t ) = 0 ;

        virtual so_audio::result play( so_audio::audio_buffer_ptr_t ) = 0 ;
        virtual so_audio::result stop( so_audio::audio_buffer_ptr_t ) = 0 ;

        virtual so_audio::result submit( so_audio::audio_buffer_ptr_t, so_audio::range_cref_t  ) = 0 ;
    };
}

#endif