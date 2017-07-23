//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPX_SYSTEM_IWINDOW_H_
#define _SNAKEOIL_GPX_SYSTEM_IWINDOW_H_

#include "../api.h"
#include "../protos.h"
#include "../typedefs.h"

#include <snakeoil/gpu/api/api_types.h>

namespace so_gpx
{
    class iwindow
    {
    public:

        /// a render thread starts execution
        virtual void_t render_thread_begin( void_t ) = 0 ;

        /// a render thread end execution 
        virtual void_t render_thread_end( void_t ) = 0 ;

        /// at the end of each frame 
        virtual void_t end_frame( void_t ) = 0 ;

    public:

        virtual so_gpu::iapi_ptr_t get_api( void_t ) = 0 ;
        virtual so_gpu::api_type get_api_type( void_t ) = 0 ;
    };
}

#endif