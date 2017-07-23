//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_COMPONENTS_SCROLL_H_
#define _SNAKEOIL_DEVICE_COMPONENTS_SCROLL_H_

#if 0

#include "../comp.h"

namespace so_device
{
    namespace so_component
    {
        /// we define a scroll component as something
        /// that can scroll left/right or up/down
        /// @note relative movement
        struct scroll : public comp<scroll>
        {
            size_t cid = size_t( -1 ) ;            

            /// - : down, left scroll
            /// + : up, right scroll
            /// 0 : no scroll
            float_t intensity = 0.0f ;
        };
        comp_type const comp<scroll>::ct = comp_type::scroll ;

        so_typedef( scroll ) ;
    }
}

#endif

#endif
