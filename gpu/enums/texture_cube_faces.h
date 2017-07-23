//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_TEXTURE_CUBE_FACES_H_
#define _SNAKEOIL_GPU_TEXTURE_CUBE_FACES_H_

#include <string>

namespace so_gpu
{
    enum class cube_face
	{
		pos_x,
		neg_x,
		pos_y,
		neg_y,
		pos_z,
		neg_z
	} ;

	namespace so_internal
    {
        static const std::string cube_face_string_array[] = 
        {
            "pos_x","neg_x","pos_y","neg_y","pos_z","neg_z"
        } ;
    }

	static std::string const & to_string( cube_face cf )
    {
		return so_internal::cube_face_string_array[size_t(cf)] ;
	}
    
}

#endif

