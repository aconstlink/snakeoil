//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_ENUMS_IMAGE_FORMATS_H_
#define _SNAKEOIL_GPU_ENUMS_IMAGE_FORMATS_H_

#include <string>

namespace so_gpu
{
    enum class image_format
	{
		undefined,
		depth_component,
		depth24_stencil8,
		intensity,
		rgb,
		rgba
	} ;
}

#endif

