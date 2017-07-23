//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_API_XAUDIO2_PCM_INFO_H_
#define _SNAKEOIL_AUDIO_API_XAUDIO2_PCM_INFO_H_

#include "../../typedefs.h"
#include "../../buffer/pcm/pcm_types.h"

namespace so_audio
{
    namespace so_xaudio2
    {
        struct pcm_info
        {
            enum class sample_type
            {
                int8,
                int16,
                int32,
                ieee_float,
                invalid
            };

            sample_type type ;
            size_t channels ;
            size_t samples_per_sec ;
            size_t seconds ;

            static sample_type deduce( so_audio::buffer_type const bt )
            {
                switch( bt )
                {
                    case so_audio::buffer_type::ieee_float:
                        return sample_type::ieee_float ;
                    case so_audio::buffer_type::int8:
                        return sample_type::int8 ;
                    case so_audio::buffer_type::int16 :
                        return sample_type::int16 ;
                    case so_audio::buffer_type::int32 :
                        return sample_type::int32 ;
                    default:
                        break ;
                }
                return sample_type::invalid ;
            }
        };
        so_typedef( pcm_info ) ;
    }    
}

#endif