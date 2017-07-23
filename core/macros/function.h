//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_CORE_MACROS_FUNCTION_H_
#define _SNAKEOIL_CORE_MACROS_FUNCTION_H_

#include "common.h"

#if defined( SNAKEOIL_COMPILER_MSC )

    #define SNAKEOIL_FUNC __FUNCTION__
    #define SNAKEOIL_LINE __LINE__

#elif defined( SNAKEOIL_COMPILER_GNU )

    #define SNAKEOIL_FUNC "Func"
    #define SNAKEOIL_LINE __LINE__

#else

    #define SNAKEOIL_FUNC "SNAKEOIL_FUNC" 
    #define SNAKEOIL_LINE 0

#endif

#endif



