//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_STD_SNAKEOIL_API_H_
#define _SNAKEOIL_STD_SNAKEOIL_API_H_

#if defined( SNAKEOIL_BUILD_DLL )

    #if defined( SNAKEOIL_STD_EXPORT )
        #define SNAKEOIL_IMEX_API_C extern "C" __declspec(dllexport)
    #define SNAKEOIL_IMEX_API __declspec(dllexport)
    #else
        #define SNAKEOIL_IMEX_API_C extern "C" __declspec(dllimport)
    #define SNAKEOIL_IMEX_API __declspec(dllimport)
    #endif    

#else 

    #define SNAKEOIL_IMEX_API_C 
    #define SNAKEOIL_IMEX_API 

#endif 
#endif // GUARD

