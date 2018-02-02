//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPX_APPLICATION_UPDATE_DATA_H_
#define _SNAKEOIL_APPX_APPLICATION_UPDATE_DATA_H_

#include "../typedefs.h"

namespace so_appx
{
    struct update_data
    {
        /// the delta time in microseconds granularity
        uint64_t microseconds = 0u ;

        /// the delta time in milliseconds granularity
        uint64_t milliseconds = 0u ;

        /// the delta time in seconds granularity
        uint64_t seconds = 0u ;

        /// the delta time in seconds, with fractional
        double_t dt = 0.0 ;

        /// smoothed delta time in seconds
        double_t dts = 0.0 ;

    };
    so_typedef( update_data ) ;
}

#endif

