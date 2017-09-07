//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPX_SYSTEM_SYSTEM_STRUCTS_H_
#define _SNAKEOIL_GPX_SYSTEM_SYSTEM_STRUCTS_H_

#include "../typedefs.h"

namespace so_gpx
{
    struct schedule_instance
    {
        size_t render_id = 0 ;
    };
    so_typedef( schedule_instance ) ;
}

#endif