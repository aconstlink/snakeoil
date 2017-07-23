//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPX_RESULT_H_
#define _SNAKEOIL_APPX_RESULT_H_

#include <snakeoil/core/assert.h>
#include <string>

namespace so_appx
{
    enum result 
    {
        ok,
        failed,
        terminate,
        not_ready,
        failed_gfx_context_creation,
        failed_gfx_context_release,
        failed_render_thread_creation,
        failed_wgl,
        failed_glx_version,
        invalid_win32_handle,
        invalid_window_handle,
        invalid_window_name,
        invalid_argument,
        invalid_gfx_api_version,
        invalid_extension,
        thread_running,
        thread_stoped,
        not_implemented,
        no_decorated,
        appx_activate,
        appx_deactivate,
        appx_background,
        appx_switch,
        num_results
    };
    
    static bool success( result res )
    {
        so_assert( res < num_results ) ;
        return res == ok ;
    }

    static bool no_success(result res) 
    {
        return !success(res);
    }

    static const std::string _app_result_strings[] = 
    {
        "so_appx::result"
    } ;

    /// not implemented yet.
    static std::string const & to_string( result /*res*/ ) 
    {
        return _app_result_strings[0] ;
    }
}
#endif

