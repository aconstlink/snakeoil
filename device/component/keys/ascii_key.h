//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../typedefs.h"
#include <snakeoil/log/log.h>

namespace so_device
{
    enum class ascii_key
    {
        invalid,
        escape,
        f1,
        f2,
        f3,
        f4,
        f5,
        f6,
        f7,
        f8,
        f9,
        f10,
        f11,
        f12,
        f13,
        f14,
        print,
        pause,
        a,
        b,
        c,
        d,
        e,
        f,
        g,
        h,
        i,
        j,
        k,
        l,
        m,
        n,
        o,
        p,
        q,
        r,
        s,
        t,
        u,
        v,
        w,
        x,
        y,
        z,
        k_0,
        k_1,
        k_2,
        k_3,
        k_4,
        k_5,
        k_6,
        k_7,
        k_8,
        k_9,
        num_0,
        num_1,
        num_2,
        num_3,
        num_4,
        num_5,
        num_6,
        num_7,
        num_8,
        num_9,
        num_komma,
        num_sub,
        num_add,
        num_mult,
        num_div,
        num_caps,
        num_return,
        shift_left,
        shift_right,
        ctrl_left,
        ctrl_right,
        alt_left,
        alt_right,
        k_return,
        back_space,
        space,
        tab,
        tab_lock,
        sharp,
        plus,
        minus,
        komma,
        point,
        insert,
        k_delete,
        pos1,
        sk_end,
        one_up,
        one_down,
        arrow_left,
        arrow_right,
        arrow_up,
        arrow_down,
        lower_than,
        // os specific?
        context,
        // windows spec
        win_win_left,
        win_win_right,
        // mac spec
        mac_control,

        // last key
        num_keys
    };

    static so_device::ascii_key convert_ascii_number_keys( size_t const delta )
    {
        bool_t const cond = delta <= ( size_t( ascii_key::k_9 ) - size_t( ascii_key::k_0 ) ) ;
        so_log::log::error_and_exit( so_core::is_not( cond ),
            "[so_device::convert_ascii_number_keys]" ) ;

        return ascii_key( size_t( ascii_key::k_0 ) + delta ) ;
    }

    static ascii_key convert_ascii_letter_keys( size_t const delta )
    {
        bool_t const cond = delta <= ( size_t( ascii_key::z ) - size_t( ascii_key::a ) ) ;
        so_log::log::error_and_exit( so_core::is_not( cond ),
            "[so_device::convert_ascii_letter_keys]" ) ;

        return ascii_key( size_t( ascii_key::a ) + delta ) ;
    }

    static ascii_key convert_ascii_function_keys( size_t const delta )
    {
        bool_t const cond = delta <= ( size_t( ascii_key::f14 ) - size_t( ascii_key::f1 ) ) ;
        so_log::log::error_and_exit( so_core::is_not( cond ),
            "[so_device::convert_ascii_function_keys]" ) ;

        return ascii_key( size_t( ascii_key::f1 ) + delta ) ;
    }

    static ascii_key convert_ascii_numpad_number_keys( size_t const delta )
    {
        bool_t const cond = delta <= ( size_t( ascii_key::num_9 ) - size_t( ascii_key::num_0 ) ) ;
        so_log::log::error_and_exit( so_core::is_not( cond ),
            "[so_device::convert_ascii_numpad_number_keys]" ) ;

        return ascii_key( size_t( ascii_key::num_0 ) + delta ) ;
    }
}