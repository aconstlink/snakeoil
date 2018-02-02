//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_RENDER_STATES_H_
#define _SNAKEOIL_GPU_RENDER_STATES_H_

#include <string>

namespace so_gpu
{
    enum class render_state
    {
        undefined,
        clearscreen, // ?
        cullmode,
        frontface,
        fillmode,
        blend,
        depth_test,
        depth_write,
        scissor_test
    } ;

    namespace so_internal
    {
        static std::string const render_state_strings[] = 
        {
            "undefined","clearscreen", "cullmode", "frontface", "fillmode",
            "blend","depth_test","depth_write", "scissor_test"
        } ;
    }

    static std::string const & to_string( render_state rs ) 
    {
        return so_internal::render_state_strings[size_t(rs)] ;
    }
}

#endif


