//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_COMMAND_SYSTEM_STATE_H_
#define _SNAKEOIL_THREAD_COMMAND_SYSTEM_STATE_H_

namespace so_thread
{
	enum command_system_state
	{
		css_proceed,
		css_wait,
		css_preserve_state
	};
}

#endif

