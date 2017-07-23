//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_API_API_TYPES_H_
#define _SNAKEOIL_AUDIO_API_API_TYPES_H_

namespace so_audio
{
    enum class api_type
    {
        null,
        wasapi,
        xaudio2
    };

    namespace so_internal
    {
        static const so_std::string_t api_type_short_names[] =
        {
            "null", "wasapi", "xaudio2"
        } ;

        static const so_std::string_t api_type_long_names[] =
        {
            "NULL", "Win32 Core Audio Api", "XAudio 2 Api"
        } ;
    }

    static so_std::string_cref_t to_string( api_type const t )
    {
        return so_internal::api_type_short_names[size_t( t )] ;
    }

    static so_std::string_cref_t to_long_string( api_type const t )
    {
        return so_internal::api_type_long_names[size_t( t )] ;
    }
}

#endif