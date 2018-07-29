//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#if defined( SNAKEOIL_TARGET_OS_WIN )

    #if defined( SNAKEOIL_PROJECT_CARS2_EXPORT )
    #define SNAKEOIL_PROJECT_CARS2_API_C extern "C" __declspec(dllexport)
    #define SNAKEOIL_PROJECT_CARS2_API __declspec(dllexport)
    #elif defined( SNAKEOIL_PROJECT_CARS2_IMPORT )
    #define SNAKEOIL_PROJECT_CARS2_API_C extern "C" __declspec(dllimport)
    #define SNAKEOIL_PROJECT_CARS2_API __declspec(dllimport)
    #else
    #define SNAKEOIL_PROJECT_CARS2_API_C
    #define SNAKEOIL_PROJECT_CARS2_API
    #endif    

#else 

    #define SNAKEOIL_PROJECT_CARS2_API_C
    #define SNAKEOIL_PROJECT_CARS2_API

#endif // SNAKEOIL_TARGET_OS_WIN


