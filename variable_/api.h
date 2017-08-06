//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_VARIABLE_SNAKEOIL_API_H_
#define _SNAKEOIL_VARIABLE_SNAKEOIL_API_H_

#if defined( SNAKEOIL_BUILD_DLL )

    #if defined( SNAKEOIL_VARIABLE_EXPORT )
    #define SNAKEOIL_VARIABLE_API_C extern "C" __declspec(dllexport)
    #define SNAKEOIL_VARIABLE_API __declspec(dllexport)
    #else
    #define SNAKEOIL_VARIABLE_API_C extern "C" __declspec(dllimport)
    #define SNAKEOIL_VARIABLE_API __declspec(dllimport)
    #endif    

#else

    #define SNAKEOIL_VARIABLE_API_C
    #define SNAKEOIL_VARIABLE_API

#endif

#endif // GUARD

