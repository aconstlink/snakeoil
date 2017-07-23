//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIOX_API_IPLUG_FACTORY_H_
#define _SNAKEOIL_AUDIOX_API_IPLUG_FACTORY_H_

#include "iplug.h"

#include <snakeoil/audio/api/api_types.h>
#include <snakeoil/std/string/string.hpp>

namespace so_audiox
{
    class SNAKEOIL_AUDIOX_API iplug_factory
    {
    public:

        virtual so_audiox::iplug_ptr_t create_plug( so_audio::api_type const, so_audio::iapi_ptr_t,
            so_audiox::iglobal_service_ptr_t, so_audiox::ilocal_service_ptr_t ) = 0 ;

        virtual void_t destroy_plug( so_audiox::iplug_ptr_t ) = 0 ;

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( iplug_factory ) ;
}

#endif