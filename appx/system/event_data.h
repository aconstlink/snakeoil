//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPX_DATA_EVENT_DATA_H_
#define _SNAKEOIL_APPX_DATA_EVENT_DATA_H_

#include "../api.h"
#include "../typedefs.h"
#include "../result.h"
#include "../protos.h"

namespace so_appx
{
    struct window_event_data
    {
        size_t wid ;
        iwindow_state_informer_ptr_t wsi ;
    };
    so_typedef( window_event_data ) ;

}

#endif

