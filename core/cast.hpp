//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_CORE_CAST_HPP_
#define _SNAKEOIL_CORE_CAST_HPP_

#include "checks.h"

namespace so_core
{
    /// @params B base class
    /// @params T concrete class
    template< typename To, typename From >
    bool_t can_cast( From ptr )
    {
        return so_core::is_not_nullptr(dynamic_cast< To >( ptr )) ;
    }
    
    
}

#endif


