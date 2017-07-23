//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_RENDER_WINDOW_WGL_WGL_CONTEXT_H_
#define _SNAKEOIL_APPLICATION_RENDER_WINDOW_WGL_WGL_CONTEXT_H_

#include "../irender_context.h"
#include "../iwindow_handle.h"
#include "../gl_info.h"

#include <snakeoil/math/vector/vector4.hpp>
#include <snakeoil/gpu/api/gl/gl_any/gl_context.h>

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/vector.hpp>

#include <windows.h>

namespace so_app
{
    namespace so_win32
    {
        class SNAKEOIL_APPLICATION_API wgl_context : public irender_context, 
            public so_gpu::so_gl::gl_context
        {
            so_this_typedefs( wgl_context ) ;

        private:

            HWND _hwnd = NULL ;
            HGLRC _hrc  = NULL ;

            /// will only be used temporarily for making the
            /// context active. Will be released on deactivation.
            HDC _hdc = NULL ;   

        private:

            /// copy construction is not allowed.
            /// a render context is a unique object.
            wgl_context( this_cref_t ) {}

        public:
        
            /// initializes this context but does not create the context.
            /// @see create_context
            wgl_context( void_t ) ;

            /// allows to move-construct a context.
            wgl_context( this_rref_t ) ;
            virtual ~wgl_context( void_t ) ;

        private:

            wgl_context( HWND hwnd, HGLRC ctx ) ;

        public: // operator =

            this_ref_t operator = ( this_cref_t ) = delete ;
            this_ref_t operator = ( this_rref_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_app::result activate( void_t ) ;
            virtual so_app::result deactivate( void_t ) ;
            virtual so_app::result vsync( bool_t on_off ) ;
            virtual so_app::result swap( void_t ) ;
            virtual irender_context_ptr_t create_shared_context( void_t ) ;

        public:

            /// @note a valid window handle must be passed.
            so_app::result create_context( gl_info_cref_t gli, iwindow_handle_ptr_t wnd_ptr ) ;
            
            /// allows to create a shared context for supporting the main context.
            /// this context can not be associated with a separate window.
            this_ptr_t create_shared( void_t ) ;
            void_t destroy_shared( this_ptr_t ) ;

            /// Returns ok, if the extension is supported, otherwise, this function fails.
            /// @precondition Must be used after context has been created and made current.
            so_app::result is_extension_supported( so_std::string_cref_t extension_name ) ;

            /// This function fills the incoming list with all wgl extension strings.
            /// @precondition Must be used after context has been created and made current.
            so_app::result get_wgl_extension( so_std::vector< so_std::string_t > & ext_list ) ;

            /// @precondition Context must be current.
            so_app::result get_gl_extension( so_std::vector< so_std::string_t > & ext_list ) ;

            /// @precondition Must be used after context has been created and made current. 
            result get_gl_version( so_app::gl_version & version ) const ;

            /// @precondition Context must be active. For debug purpose. Just clears the screen.
            void_t clear_now( so_math::vec4f_t const & vec ) ;

        private:

            HWND get_win32_handle( iwindow_handle_ptr_t ) ;

            so_app::result create_the_context( gl_info_cref_t gli ) ;
            HGLRC create_the_shared( void_t ) ;

        };
        so_typedefs( wgl_context, wgl_context ) ;
    }    
}

#endif

