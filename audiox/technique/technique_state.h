//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIOX_TECHNIQUE_TECHNIQUE_STATE_H_
#define _SNAKEOIL_AUDIOX_TECHNIQUE_TECHNIQUE_STATE_H_

namespace so_audiox
{
    enum class technique_state
    {
        
        raw,
        loading,
        init,
        
        ready,
        transfer,
        updated,
        execute,
        executing,
        execute_done,
        release,
        failed,
    };
}

#endif