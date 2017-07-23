//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_LOG_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_LOG_H_

#include "../../../typedefs.h"
#include <snakeoil/gli/gl/gl.h>
#include <snakeoil/log/log.h>

namespace so_gpu
{
    namespace so_gl
    {
        class log
        {
            typedef log this_t ;

        public:

            /// log level: error
            /// opengl specific error checking and logging.
            static bool_t error( const char * msg ) ;
            static bool_t error( std::string const & msg ) ;
            static bool_t error( bool_t condition, GLenum gle, const char * msg ) ;
            static bool_t error( bool_t condition, GLenum gle, std::string const & msg ) ;
        } ;        
    }
}

#endif

