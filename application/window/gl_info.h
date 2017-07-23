//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_WINDOW_GL_INFO_H_
#define _SNAKEOIL_APPLICATION_WINDOW_GL_INFO_H_

#include "../typedefs.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_app
{
    struct gl_version
    {
        GLint major = 4 ;
        GLint minor = 0 ;
    };

    struct gl_info
    {
        gl_version version ;

        /// RGB color bits.
        size_t color_bits = 24 ;

        /// depth bits.
        size_t depth_bits = 24 ;

        /// Specifies if double buffering should
        /// be used.
        bool_t double_buffer = true ;

        /// Allows to dis-/enable the vsync.
        bool_t vsync_enabled = true ;
    };
    so_typedefs( gl_info, gl_info ) ;	
}

#endif

