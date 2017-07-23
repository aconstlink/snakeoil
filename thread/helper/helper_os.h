//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_HELPER_OS_H_
#define _SNAKEOIL_THREAD_HELPER_OS_H_

#if defined( SNAKEOIL_TARGET_OS_WIN )
#include "win32/win32_helper.h"
namespace so_thread
{
	typedef so_thread::so_win32::helper helper_t ;
}
#elif defined( SNAKEOIL_TARGET_OS_LIN )
#include "unix/pthread_helper.h"
namespace so_thread
{
	typedef so_thread::so_unix::so_pthread::helper helper_t ;
}
#else
#error "Helper not available on this plattform."
#endif


#endif

