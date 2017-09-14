//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_VARIABLE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_OBJECTS_GL_DRIVER_OBJECTS_VARIABLE_H_

#include "../../../api_object.h"
#include "../../../../protos.h"

#include <snakeoil/gli/gl/gl.h>

#include <functional>

namespace so_gpu
{
    namespace so_gl
    {
        /// will hold the data for the uniform variable
        struct variable : public api_object
        {
            typedef std::function< void_t (void_ptr_t, variable_ptr_t)> copy_funk_t ;

            /// (location, count, data)
            typedef std::function< void_t ( GLuint, GLuint, void_ptr_t ) > uniform_funk_t ;

            variable( void_t ) {}
            variable( variable && rhv )
            {
                data_ptr = rhv.data_ptr ;
                rhv.data_ptr = nullptr ;
                do_copy = std::move( rhv.do_copy ) ;
                do_uniform = std::move( rhv.do_uniform ) ;
                loc = rhv.loc ;
            }

            /// used for loading the user defined value into
            /// driver space.
            copy_funk_t do_copy ;
            /// constructed at creation time used for issuing
            /// the copied driver value to the GL.
            uniform_funk_t do_uniform ;

            /// the uniform loaction of the program's 
            /// shader variable.
            GLuint loc = GLuint(-1) ;

            // data ptr
            // the driver impl is required to allocate 
            // the amount of memory for the type.
            void_ptr_t data_ptr = nullptr ;
        };
    }
}

#endif

