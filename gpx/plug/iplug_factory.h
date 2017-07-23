//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPX_API_IPLUG_FACTORY_H_
#define _SNAKEOIL_GPX_API_IPLUG_FACTORY_H_

#include "iplug.h"

#include <snakeoil/gpu/api/api_types.h>
#include <snakeoil/std/string/string.hpp>

namespace so_gpx
{
    class SNAKEOIL_GPX_API iplug_factory
    {
    public:

        virtual so_gpx::iplug_ptr_t create_plug( so_gpu::api_type const, so_gpu::iapi_ptr_t ) = 0 ;

        virtual void_t destroy_plug( so_gpx::iplug_ptr_t ) = 0 ;

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( iplug_factory ) ;
}

#endif