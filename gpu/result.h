//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_RESULT_H_
#define _SNAKEOIL_GPU_RESULT_H_

#include "typedefs.h"
#include <string>

namespace so_gpu
{
    enum result
    {
        ok,
        failed,
        invalid,
        ignore,
        invalid_argument,
        invalid_program,
        invalid_shader,
        invalid_atom,

        failed_vertex_buffer,
        failed_index_buffer,
        failed_shader,
        failed_shader_compile,
        failed_shader_link,
        failed_gl_api,
        failed_type_cast,
        available,
        not_available,
        mt_processing,
        user_0,
        num_results
    };

    typedef result * result_ptr_t ;

    namespace so_internal
    {
        static const std::string __graphics_result_strings[] = {
            "so_gpu::result"
        } ;
    }

    /// not implemented yet.
    static std::string const & to_string( result /*res*/ ) 
    {
        return so_internal::__graphics_result_strings[0] ;
    }

    static bool_t success( so_gpu::result res )
    {
        return res == ok ;
    }

    static bool_t no_success( result res )
    {
        return !success( res ) ;
    }
}

#endif

