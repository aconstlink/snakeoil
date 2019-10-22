//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#if defined( SNAKEOIL_BUILD_DLL )

    #if defined( SNAKEOIL_EVENT_EXPORT )
        #define SNAKEOIL_EVENT_API_C extern "C" __declspec(dllexport)
        #define SNAKEOIL_EVENT_API __declspec(dllexport)
    #else
        #define SNAKEOIL_EVENT_API_C extern "C" __declspec(dllimport)
        #define SNAKEOIL_EVENT_API __declspec(dllimport)
    #endif    

#else 

    #define SNAKEOIL_EVENT_API_C 
    #define SNAKEOIL_EVENT_API 

#endif // SNAKEOIL_TARGET_OS_WIN


