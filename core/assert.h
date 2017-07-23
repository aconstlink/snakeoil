//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_CORE_ASSERT_H_
#define _SNAKEOIL_CORE_ASSERT_H_

#if defined( SNAKEOIL_DEBUG )

#include <assert.h>
#define so_assert(x) assert(x)

#else

#define so_assert(x) if(!(x)){/*do critical output and exit*/}((void)0)

#endif

#endif



