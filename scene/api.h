//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_SNAKEOIL_API_H_
#define _SNAKEOIL_SCENE_SNAKEOIL_API_H_

#if defined( SNAKEOIL_TARGET_OS_WIN )

    #if defined( SNAKEOIL_SCENE_EXPORT )
    #define SNAKEOIL_SCENE_API_C extern "C" __declspec(dllexport)
    #define SNAKEOIL_SCENE_API __declspec(dllexport)
    #elif defined( SNAKEOIL_SCENE_IMPORT )
    #define SNAKEOIL_SCENE_API_C extern "C" __declspec(dllimport)
    #define SNAKEOIL_SCENE_API __declspec(dllimport)
    #else
    #define SNAKEOIL_SCENE_API_C
    #define SNAKEOIL_SCENE_API
    #endif    

#else

    #define SNAKEOIL_SCENE_API_C
    #define SNAKEOIL_SCENE_API

#endif // SNAKEOIL_TARGET_OS_WIN

#endif // GUARD

