//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_RESOURCE_SNAKEOIL_API_H_
#define _SNAKEOIL_RESOURCE_SNAKEOIL_API_H_

#if defined( SNAKEOIL_BUILD_DLL )

    #if defined( SNAKEOIL_RESOURCE_EXPORT )
    #define SNAKEOIL_RESOURCE_API_C extern "C" __declspec(dllexport)
    #define SNAKEOIL_RESOURCE_API __declspec(dllexport)
    #else
    #define SNAKEOIL_RESOURCE_API_C extern "C" __declspec(dllimport)
    #define SNAKEOIL_RESOURCE_API __declspec(dllimport)
    #endif

#else 
    
    #define SNAKEOIL_RESOURCE_API_C 
    #define SNAKEOIL_RESOURCE_API

#endif // SNAKEOIL_TARGET_OS_WIN

#endif // GUARD

