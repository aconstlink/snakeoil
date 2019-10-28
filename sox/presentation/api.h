//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#if defined( SNAKEOIL_BUILD_DLL )

    #if defined( SNAKEOILX_PRESENTATION_EXPORT )
        #define SNAKEOILX_PRESENTATION_API_C extern "C" __declspec(dllexport)
        #define SNAKEOILX_PRESENTATION_API __declspec(dllexport)
    #else
        #define SNAKEOILX_PRESENTATION_API_C extern "C" __declspec(dllimport)
        #define SNAKEOILX_PRESENTATION_API __declspec(dllimport)
    #endif    

#else 

    #define SNAKEOILX_PRESENTATION_API_C 
    #define SNAKEOILX_PRESENTATION_API 

#endif // SNAKEOIL_TARGET_OS_WIN


