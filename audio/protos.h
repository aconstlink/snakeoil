//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_PROTOS_H_
#define _SNAKEOIL_AUDIO_PROTOS_H_

#include <snakeoil/core/macros/typedef.h>

namespace so_audio
{
    so_class_proto_typedefs( api_object ) ;

    so_class_proto_typedefs( audio_system ) ;
    so_class_proto_typedefs( iapi ) ;
    so_class_proto_typedefs( ixaudio2_api ) ;
    so_class_proto_typedefs( iwasapi_api ) ;

    so_class_proto_typedefs( ibuffer ) ;
    so_class_proto_typedefs( ipcm_buffer ) ;
    so_class_proto_typedefs( loopback_buffer ) ;
    so_class_proto_typedefs( audio_buffer ) ;
}

#endif

