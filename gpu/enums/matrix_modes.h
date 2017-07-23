//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_MATRIX_MODES_H_
#define _SNAKEOIL_GPU_MATRIX_MODES_H_

#include <string>

namespace so_gpu
{
    enum class matrix_mode
    {
        undefined,
        modelview,
        projection,
        texture
    } ;

    namespace so_internal
    {
        static std::string const matrix_mode_string_array[] = 
        {
            "undefined", "modelview", "projection", "texture"
        } ;
    }

    static std::string const & to_string( matrix_mode mm )
    {
        return so_internal::matrix_mode_string_array[size_t(mm)] ;
    }
    
} 

#endif

