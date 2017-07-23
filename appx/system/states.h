//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPX_MICRO_STATES_H_
#define _SNAKEOIL_APPX_MICRO_STATES_H_

namespace so_appx
{
    enum class appx_state
    {
        invalid,
        failed,
        initializing,
        running,
        shutting_down,
        shutdown
    };
}

#endif

