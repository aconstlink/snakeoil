//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPX_TECHNIQUE_TECHNIQUE_STATE_H_
#define _SNAKEOIL_GPX_TECHNIQUE_TECHNIQUE_STATE_H_

namespace so_gpx
{
    enum class technique_rest_state
    {
        offline, 
        online,
        loaded,
        reloaded,
        unloaded,
        initialized,
        released
    };

    enum class technique_transition_state
    {
        none,
        failed,
        loading,
        reloading,
        unloading,
        initializing,
        releasing,

        updating,
        transfering,
        executing
    };

    enum class technique_schedule_goal
    {
        none,
        for_reload,
        for_online,
        for_exec,
        for_shutdown
    };

    enum class technique_transition_result
    {
        goal_reached,
        in_transition,
        state_changed,
        failed
    };

    enum class phase_state
    {
        none,
        stage_00,
        stage_01
    };
}

#endif