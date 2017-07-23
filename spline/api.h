//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SPLINE_SNAKEOIL_API_H_
#define _SNAKEOIL_SPLINE_SNAKEOIL_API_H_

#if defined( SNAKEOIL_BUILD_DLL )

    #if defined( SNAKEOIL_SPLINE_EXPORT )
    #define SNAKEOIL_MATHX_API_C extern "C" __declspec(dllexport)
    #define SNAKEOIL_MATHX_API __declspec(dllexport)
    #else
    #define SNAKEOIL_MATHX_API_C extern "C" __declspec(dllimport)
    #define SNAKEOIL_MATHX_API __declspec(dllimport)
    #endif    

#else 

    #define SNAKEOIL_MATHX_API_C
    #define SNAKEOIL_MATHX_API

#endif 

#endif // GUARD

