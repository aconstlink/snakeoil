//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_TEXTURE_FORMATS_H_
#define _SNAKEOIL_GPU_TEXTURE_FORMATS_H_

#include <string>

namespace so_gpu
{
    enum class texture_format
	{
		undefined, // undefined means "not supported"
		alpha,
		alpha4,
		alpha8,
		alpha12,
		alpha16,
		compressed_alpha,
		compressed_luminance,
		compressed_luminance_alpha,
		compressed_intensity,
		compressed_rgb,
		compressed_rgba,
		depth_component,
		depth_component16,
		depth_component24,
		depth_component32,
		depth24_stencil8,
		luminance,
		luminance4,
		luminance8,
		luminance12,
		luminance16,
		intensity,
		intensity4,
		intensity8,
		intensity12,
		intensity16,
		rgb,
		rgb4,
		rgb5,
		rgb8,
		rgb10,
		rgb12,
		rgb16,
		rgba,
		rgba2,
		rgba4,
		rgba8,
		rgba12,
		rgba16,
		rgba32f,
		rgb32f,
		alpha32f,
		intensity32f,
		luminance32f,
		luminance_alpha32f,
		rgba16f,
		rgb16f,
		alpha16f,
		intensity16f,
		luminance16f,
		luminance_alpha16f,
		r8i, 
		r8ui, 
		r32i,
		r32ui,
		rg8i,
		rg8ui,
		rg32i,
		rg32ui,
		rgb8i,
		rgb8ui,
		rgb32i,
		rgb32ui,
		rgba8i,
		rgba8ui,
		rgba32i,
		rgba32ui
	} ;

	namespace so_internal
    {
	    static const std::string texture_format_string_array_eng[] = 
        {
		    "undefined","alpha","alpha4","alpha8","alpha12","alpha16","compressed_alpha",
		    "compressed_luminance","compressed_luminance_alpha","compressed_intensity",
		    "compressed_rgb","compressed_rgba","depth_component","depth_component16",
		    "depth_component24","depth_component32","luminance","luminance4",
		    "luminance8","luminance12","luminance16","intensity","intensity4",
		    "intensity8","intensity12","intensity16","rgb","rgb4","rgb5","rgb8",
		    "rgb10","rgb12","rgb16","rgba","rgba2","rgba4","rgba8","rgba12",
		    "rgba16","rgba32f","rgb32f","alpha32f","intensity32f","luminance32f",
		    "luminance_alpha32f","rgba16f","rgb16f","alpha16f","intensity16f",
			"luminance16f","luminance_alpha16f","r8i","r8ui", "r32i","r32ui","rg8i","rg8ui",
			"rg32i", "rg32ui", "rgb8i", "rgb8ui", "rgb32i", "rgb32ui", "rgba8i", "rgba8ui",
			"rgba32i", "rgba32ui"
	    } ;
    }
	    
	static std::string const & to_string( texture_format tf )
    {
        return so_internal::texture_format_string_array_eng[size_t(tf)] ;
	}
}

#endif

