//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "button_id.h"
#include "three_button.h"

namespace so_device
{
    static button_id_t three_button_to_button( three_button tb )
    {
        switch( tb )
        {
        case three_button::left: return button_id_t( 0 ) ;
        case three_button::right: return button_id_t( 1 ) ;
        case three_button::middle: return button_id_t( 2 ) ;
        default:  break ;
        }
        return button_id_t( -1 ) ;
    }

    static three_button button_to_three_button( button_id_t const b )
    {
        switch( b )
        {
        case button_id_t( 0 ): return three_button::left ;
        case button_id_t( 1 ): return three_button::right ;
        case button_id_t( 2 ): return three_button::middle ;
        default: break ;
        }

        return three_button::none ;
    }
}
