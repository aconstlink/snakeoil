//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_GL_API_GL_GL_ANY_GL_CONVERT_TEXTURE_BUFFER_FORMAT_H_
#define _SNAKEOIL_GPU_GL_API_GL_GL_ANY_GL_CONVERT_TEXTURE_BUFFER_FORMAT_H_

#include "../../../../enums/data_buffer_formats.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const GLenum data_buffer_formats_array_gl[] =
            {
                0,					// TBF_UNDEFINED,
                0,//GL_INTENSITY8,		// TBF_INTENSITY8,
                0,//GL_INTENSITY16,		// TBF_INTENSITY16,
                0,//GL_INTENSITY16F,	// TBF_INTENSITY16F,
                0,//GL_INTENSITY32F,	// TBF_INTENSITY32F,
                0,//GL_INTENSITY8I,		// TBF_INTENSITY8I,
                0,//GL_INTENSITY16I,	// TBF_INTENSITY16I,
                0,//GL_INTENSITY32I,	// TBF_INTENSITY32I,
                0,//GL_INTENSITY8UI,	// TBF_INTENSITY8UI,
                0,//GL_INTENSITY16UI,	// TBF_INTENSITY16UI,
                0,//GL_INTENSITY32UI,	// TBF_INTENSITY32UI,
                GL_RGBA8,			// TBF_RGBA8,
                GL_RGBA16,			// TBF_RGBA16,
                GL_RGBA16F,			// TBF_RGBA16F,
                GL_RGBA32F,			// TBF_RGBA32F,
                GL_RGBA8I,			// TBF_RGBA8I,
                GL_RGBA16I,			// TBF_RGBA16I,
                GL_RGBA32I,			// TBF_RGBA32I,
                GL_RGBA8UI,			// TBF_RGBA8UI,
                GL_RGBA16UI,		// TBF_RGBA16UI,
                GL_RGBA32UI			// TBF_RGBA32UI,
            } ;
        }

        static GLenum convert( data_buffer_format const d )
        {
            return so_internal::data_buffer_formats_array_gl[size_t(d)] ;
        }
    }
}

#endif

