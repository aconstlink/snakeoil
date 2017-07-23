//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_API_XAUDIO2_XAUDIO2_H_
#define _SNAKEOIL_AUDIO_API_XAUDIO2_XAUDIO2_H_

#include "ixaudio2_api.h"

#include <xaudio2.h>

namespace so_audio
{
    namespace so_win32
    {
        class SNAKEOIL_AUDIO_API xaudio2_api : public ixaudio2_api
        {

            so_this_typedefs( xaudio2_api ) ;

        private:

            IXAudio2 * _xeng_ptr = nullptr ;
            IXAudio2MasteringVoice * _dest_ptr = nullptr ;

        public:

            xaudio2_api( void_t ) ;
            xaudio2_api( this_cref_t ) = delete ;
            xaudio2_api( this_rref_t ) ;
            virtual ~xaudio2_api( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public: // iapi interface

            virtual bool_t initialize( void_t ) ;
            virtual void_t release( void_t ) ;

            virtual api_type get_type( void_t ) const ;
            virtual void_t destroy( void_t ) ;

        public: // ixaudio2 interface

            virtual so_audio::result create( so_audio::audio_buffer_ptr_t, so_xaudio2::pcm_info_cref_t ) ;
            virtual so_audio::result release( so_audio::audio_buffer_ptr_t ) ;

            virtual so_audio::result play( so_audio::audio_buffer_ptr_t ) ;
            virtual so_audio::result stop( so_audio::audio_buffer_ptr_t ) ;

            virtual so_audio::result submit( so_audio::audio_buffer_ptr_t, so_audio::range_cref_t ) ;

        };
        so_typedef( xaudio2_api ) ;
    }
}

#endif