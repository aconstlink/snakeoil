
#pragma once

#include "../api.h"
#include "../typedefs.h"

namespace so_event
{
    enum class event_info
    {
        invalid,
        triggered,
        reset,
        no_change
    };

    class SNAKEOIL_EVENT_API itrigger
    {

    public:

        virtual void_t on_event( event_info const & ) = 0 ;
        
    };
    so_typedef( itrigger ) ;
}