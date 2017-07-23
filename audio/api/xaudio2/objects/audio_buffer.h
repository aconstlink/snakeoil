//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_API_XAUDIO2_OBJECTS_AUDIO_BUFFER_H_
#define _SNAKEOIL_AUDIO_API_XAUDIO2_OBJECTS_AUDIO_BUFFER_H_

#include "../../api_object.h"

#include <xaudio2.h>

namespace so_audio
{
    namespace so_xaudio2
    {
        struct audio_buffer : public api_object
        {
            so_this_typedefs( audio_buffer ) ;

        public:

            audio_buffer( IXAudio2SourceVoice * p ) : src_ptr( p ) {}
            audio_buffer( this_cref_t ) = delete ;
            audio_buffer( this_rref_t rhv ) { so_move_member_ptr( src_ptr, rhv ) ; }
            virtual ~audio_buffer( void_t ) {}

            IXAudio2SourceVoice * src_ptr ;
            size_t samples_per_sec = 0 ;
            size_t num_channels = 0 ;
        };
        so_typedef( audio_buffer ) ;
    }
}
#endif
