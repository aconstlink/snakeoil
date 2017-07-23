//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_TEXTURE_TYPES_H_
#define _SNAKEOIL_GPU_TEXTURE_TYPES_H_

#include <string>

namespace so_gpu
{
    enum class texture_type
	{
		undefined,
		texture_1d,
		texture_2d,
		texture_3d,
		texture_cube,
		texture_array
	} ;

	namespace so_internal
    {
        static const std::string texture_type_names[] = 
        {
            "undefined","texture_1d","texture_2d","texture_3d","texture_cube",
			"texture_array"
        } ;

        static size_t texture_type_dim_array[] = { 0,1,2,3,2,2 } ;
    }

	static std::string const & to_string( texture_type tt )
    {
		return so_internal::texture_type_names[size_t(tt)] ;
	}

	static inline size_t texture_dimension( texture_type tt )
    {
		return so_internal::texture_type_dim_array[size_t(tt)] ;
	}
    
}

#endif

