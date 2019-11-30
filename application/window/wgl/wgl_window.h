//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_RENDER_WINDOW_WGL_WGL_WINDOW_H_
#define _SNAKEOIL_APPLICATION_RENDER_WINDOW_WGL_WGL_WINDOW_H_

#include "wgl_context.h"

#include "../decorator_window.h"
#include "../win32/win32_window.h"

#include "../gl_info.h"
#include "../window_info.h"

#include <snakeoil/gpx/window/iwindow.h>
#include <snakeoil/gpu/api/gl/gl_33/gl_33_api.h>

namespace so_app
{
    namespace so_win32
    {
        /// Allows to render GL into the window. It constructs a win32 window
        /// internally and sets it as the decorated window.
        /// Furthermore, it creates a WGL context which allows calling the GL.
        /// With the context, a GL driver object is created that can be directly
        /// used for rendering with using the engine.
        class SNAKEOIL_APPLICATION_API wgl_window : public decorator_window, public so_gpx::iwindow
        {
            typedef render_window base_t ;
            so_this_typedefs( wgl_window ) ;

            typedef so_gpu::so_driver::so_gl::gl_driver_ptr_t gl_driver_ptr_t ;

            typedef so_std::vector< so_gpu::so_gl::igl_api_ptr_t > support_drivers_t ;
            typedef so_std::vector< wgl_context_ptr_t > support_contexts_t ;

        private:

            /// the rendering context using the wgl.
            wgl_context_ptr_t _context_ptr = nullptr ;
            
            bool_t _tgl_vsync = false ;
            bool_t _vsync = false ;

        private:

            so_gpu::so_gl::gl_33_api_ptr_t _gl_api_ptr = nullptr ;

        public:

            wgl_window( gl_info_cref_t gli, window_info_cref_t wi ) ;
            wgl_window( this_rref_t ) ;
            virtual ~wgl_window( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t rhv ) ;
            static void_t destroy( this_ptr_t ptr ) ;
                        
        public: // interface 

            virtual so_app::result destroy( void_t ) ;
            virtual irender_context_ptr_t get_render_context( void_t ) ;
            virtual so_gpu::iapi_ptr_t get_driver( void_t ) ;
            
            virtual irender_context_ptr_t get_support_context( size_t i ) ;
            virtual so_gpu::iapi_ptr_t get_support_driver( size_t i ) ;

            virtual void_t send_toggle_vsync( void_t ) ;

        public: // so_gpu interface

            virtual void_t render_thread_begin( void_t ) ;
            virtual void_t render_thread_end( void_t ) ;
            virtual void_t end_frame( void_t ) ;
            virtual so_gpu::iapi_ptr_t get_api( void_t ) ;
            virtual so_gpu::api_type get_api_type( void_t ) ;

        protected:

            so_app::result create_context( void_t ) ;
        };
        so_typedef( wgl_window ) ;
    }    
}

#endif

