//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

namespace so_ui
{
    enum result
    {
        ok,
        failed,
        invalid_argument,

        repeat,
        call_base,
        no_descent
    };

    static bool success( result r ){ return r == ok ; }
    static bool no_success( result r ) { return !success(r) ; }

}
