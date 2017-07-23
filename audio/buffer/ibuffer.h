//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_BUFFER_IBUFFER_H_
#define _SNAKEOIL_AUDIO_BUFFER_IBUFFER_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"

namespace so_audio
{
    class SNAKEOIL_AUDIO_API ibuffer
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
        
    };
    so_typedef( ibuffer ) ;
}

#endif