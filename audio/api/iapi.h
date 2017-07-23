//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_API_IAPI_H_
#define _SNAKEOIL_AUDIO_API_IAPI_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"
#include "../result.h"

#include "api_types.h"

namespace so_audio
{
    class SNAKEOIL_AUDIO_API iapi
    {

    public:

        virtual bool_t initialize( void_t ) = 0 ;
        virtual void_t release( void_t ) = 0 ;

        virtual api_type get_type( void_t ) const = 0 ;
        virtual void_t destroy( void_t ) = 0 ;

    };
}

#endif