//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#if defined( SNAKEOIL_TARGET_OS_WIN )

    #if defined( SNAKEOIL_UI_EXPORT )
    #define SNAKEOIL_UI_API_C extern "C" __declspec(dllexport)
    #define SNAKEOIL_UI_API __declspec(dllexport)
    #elif defined( SNAKEOIL_UI_IMPORT )
    #define SNAKEOIL_UI_API_C extern "C" __declspec(dllimport)
    #define SNAKEOIL_UI_API __declspec(dllimport)
    #else
    #define SNAKEOIL_UI_API_C
    #define SNAKEOIL_UI_API
    #endif    

#else

    #define SNAKEOIL_UI_API_C
    #define SNAKEOIL_UI_API

#endif // SNAKEOIL_TARGET_OS_WIN



