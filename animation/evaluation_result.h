//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_ANIMATION_EVALUATION_RESULT_H_
#define _SNAKEOIL_ANIMATION_EVALUATION_RESULT_H_

#include "typedefs.h"

namespace so_ani
{
    enum class evaluation_result
    {
        invalid,
        out_of_range,
        in_range
    };

    static bool_t is_value_usable( evaluation_result const ev )
    {
        return ev != evaluation_result::invalid ;
    }

    static bool_t is_in_range( evaluation_result const ev )
    {
        return ev == evaluation_result::in_range ;
    }

    static bool_t is_out_of_range( evaluation_result const ev )
    {
        return ev == evaluation_result::out_of_range ;
    }
}

#endif