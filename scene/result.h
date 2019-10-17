//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_RESULT_H_
#define _SNAKEOIL_SCENE_RESULT_H_

namespace so_scene
{
    enum class result
    {
        ok,
        failed,
        invalid_argument,

        repeat
    };

    static bool success( result r ){ return r == result::ok ; }
    static bool no_success( result r ) { return !success(r) ; }

}

#endif

