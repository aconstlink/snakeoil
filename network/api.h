//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NETWORK_SNAKEOIL_API_H_
#define _SNAKEOIL_NETWORK_SNAKEOIL_API_H_

#if defined( SNAKEOIL_BUILD_DLL )

    #if defined( SNAKEOIL_NETWORK_EXPORT )
    #define SNAKEOIL_NETWORK_API_C extern "C" __declspec(dllexport)
    #define SNAKEOIL_NETWORK_API __declspec(dllexport)
    #else
    #define SNAKEOIL_NETWORK_API_C extern "C" __declspec(dllimport)
    #define SNAKEOIL_NETWORK_API __declspec(dllimport)
    #endif    

#else 

    #define SNAKEOIL_NETWORK_API_C
    #define SNAKEOIL_NETWORK_API

#endif // SNAKEOIL_TARGET_OS_WIN

#endif // GUARD

