//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#if defined( SNAKEOIL_BUILD_DLL )

    #if defined( SNAKEOIL_DEGUI_EXPORT )
    #define SNAKEOIL_DEGUI_API_C extern "C" __declspec(dllexport)
    #define SNAKEOIL_DEGUI_API __declspec(dllexport)
    #else
    #define SNAKEOIL_DEGUI_API_C extern "C" __declspec(dllimport)
    #define SNAKEOIL_DEGUI_API __declspec(dllimport)
    #endif

#else

    #define SNAKEOIL_DEGUI_API_C 
    #define SNAKEOIL_DEGUI_API

#endif // SNAKEOIL_TARGET_OS_WIN


