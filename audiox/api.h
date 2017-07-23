//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIOX_SNAKEOIL_API_H_
#define _SNAKEOIL_AUDIOX_SNAKEOIL_API_H_

#if defined( SNAKEOIL_BUILD_DLL )

    #if defined( SNAKEOIL_AUDIOX_EXPORT )
    #define SNAKEOIL_AUDIOX_API_C extern "C" __declspec(dllexport)
    #define SNAKEOIL_AUDIOX_API __declspec(dllexport)
    #else 
    #define SNAKEOIL_AUDIOX_API_C extern "C" __declspec(dllimport)
    #define SNAKEOIL_AUDIOX_API __declspec(dllimport)
    #endif

#else

    #define SNAKEOIL_AUDIOX_API_C 
    #define SNAKEOIL_AUDIOX_API 

#endif 

#endif // GUARD

