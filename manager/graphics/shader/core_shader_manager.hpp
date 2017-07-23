//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_SHADER_CORE_SHADER_MANAGER_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_SHADER_CORE_SHADER_MANAGER_H_

#include "../../result.h"
#include "../../typedefs.h"
#include "../../protos.h"

#include <snakeoil/gpu/shader/shader_types.h>
#include <snakeoil/gpu/protos.h>

#include <snakeoil/gpx/protos.h>
#include <snakeoil/gpx/event/event.h>
#include <snakeoil/gpx/system/iuser_system.h>
#include <snakeoil/gpx/driver/driver_async.h>
#include <snakeoil/thread/typedefs.h>

namespace so_manager
{
    namespace so_graphics
    {
        /// not thread safe. requires parent to be thread safe.
        template< typename shader_t >
        class core_shader_manager
        {
            so_this_typedefs( core_shader_manager<shader_t> ) ;
            
            typedef shader_t * shader_ptr_t ;

        private:

            struct shader_data
            {
                shader_ptr_t shader_ptr = nullptr ;
                /// required to be on heap to to move support.
                so_gpx::event_ptr_t compilation_status = nullptr ;
            };
            
            typedef so_std::map< so_manager::key_t, shader_data > key_to_shader_t ;
            
        private:

            /// required for shader compilation.
            so_gpx::iuser_system_ptr_t _gpxs_ptr = nullptr ;

            key_to_shader_t _key_to_shader ;
            
        private: // shutdown

            /// for shutting down the manager's shaders
            key_to_shader_t  _shutdown_keys ;

            so_gpx::event _shutdown_ready ;

        private:

            // no copy support
            core_shader_manager( this_cref_t ){}
            this_ref_t operator = ( this_cref_t rhv ) {}

        public:

            core_shader_manager( so_gpx::iuser_system_ptr_t ptr ) 
            {
                _gpxs_ptr = ptr ;

                _shutdown_ready.reset( so_gpu::invalid ) ;
            }

            core_shader_manager( this_rref_t rhv ) :
                _key_to_shader( std::move(rhv._key_to_shader) )
            {
                _gpxs_ptr = rhv._gpxs_ptr ;
                rhv._gpxs_ptr = nullptr ;

                _shutdown_ready = std::move( rhv._shutdown_ready ) ;
            }

            this_ref_t operator = ( this_rref_t rhv )
            {
                _gpxs_ptr = rhv._gpxs_ptr ;
                rhv._gpxs_ptr = nullptr ;

                _key_to_shader = std::move( rhv._key_to_shader ) ;

                _shutdown_ready = std::move( rhv._shutdown_ready ) ;
            }

            /// will delete all shader objects. 
            /// Be sure to call shutdown first. 
            /// @see shutdown, shutdown_ready
            ~core_shader_manager( void_t ) 
            {
                for( auto item : _shutdown_keys )
                {
                    so_gpu::memory::dealloc( item.second.shader_ptr ) ;
                    so_gpu::memory::dealloc( item.second.compilation_status ) ;
                }
                _shutdown_keys.clear() ;
            }

        public:

            /// this function will not destroy any objects. 
            /// it only released the shader object's driver object.
            so_manager::result shutdown( void_t ) 
            {
                _shutdown_keys = std::move(_key_to_shader) ;
                _shutdown_ready.reset( so_gpu::mt_processing ) ;
                
                _gpxs_ptr->execute( [&]( so_gpx::so_driver::driver_async_ptr_t drv )
                {
                    for( auto item : _shutdown_keys )
                    {
                        drv->release_shader( item.second.shader_ptr ) ;
                    }

                    drv->flush() ;
                    drv->finish( &_shutdown_ready ) ;
                } ) ;

                return so_manager::ok ;
            }

            so_manager::result shutdown_ready( void_t ) 
            {
                return _shutdown_ready.get_result() == so_gpu::mt_processing ?
                    so_manager::not_ready : so_manager::ok ;
            }

        public:

            so_manager::result add_shader( so_manager::key_cref_t key_in, so_manager::string_cref_t code_in ) 
            {
                auto iter = _key_to_shader.find( key_in ) ;
                if( iter != _key_to_shader.end() ) 
                    return so_manager::invalid_argument ;

                shader_data sd ;
                sd.shader_ptr = so_gpu::memory::alloc( shader_t(), 
                    "[core_shader_manager::add_shader] : shader" ) ;
                sd.shader_ptr->set_code( code_in ) ;

                sd.compilation_status = so_gpu::memory::alloc( so_gpx::event(), 
                    "[core_shader_manager::add_shader] : event" ) ;

                _key_to_shader[key_in] = sd ;

                _gpxs_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
                {
                    drv_ptr->create_shader( sd.shader_ptr ) ;
                } ) ;

                return so_manager::ok ;
            }

            so_manager::result replace_code( so_manager::key_cref_t key_in, so_manager::string_cref_t code_in ) 
            {
                auto iter = _key_to_shader.find( key_in ) ;
                if( iter == _key_to_shader.end() ) 
                {
                    return this_t::add_shader( key_in, code_in ) ;
                }

                iter->second.shader_ptr->set_code( code_in ) ;
                
                return so_manager::ok ;
            }

            so_manager::result compile_shader( so_manager::key_cref_t key_in ) 
            {
                auto iter = _key_to_shader.find( key_in ) ;
                if( iter == _key_to_shader.end() ) return so_manager::invalid_argument ;

                iter->second.compilation_status->reset( so_gpu::mt_processing ) ;
                _gpxs_ptr->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
                {
                    drv_ptr->compile( iter->second.shader_ptr, iter->second.compilation_status ) ;
                } ) ;

                return so_manager::ok ;
            }

            so_gpu::result get_compilation_status( so_manager::key_cref_t key_in ) 
            {
                auto iter = _key_to_shader.find( key_in ) ;
                if( iter == _key_to_shader.end() ) return so_gpu::invalid_argument ;

                return iter->second.compilation_status->get_result() ;
            }

            so_gpu::result wait_for_compilation( so_manager::key_cref_t key_in ) 
            {
                auto iter = _key_to_shader.find( key_in ) ;
                if( iter == _key_to_shader.end() ) return so_gpu::invalid_argument ;

                return iter->second.compilation_status->wait_for_signal() ;

            }

            shader_ptr_t get_shader( so_manager::key_cref_t key_in ) 
            {
                auto iter = _key_to_shader.find( key_in ) ;
                if( iter == _key_to_shader.end() ) return nullptr ;

                return iter->second.shader_ptr ;
            }

            so_gpx::event_ptr_t get_compilation_status_event( so_manager::key_cref_t key_in )
            {
                auto iter = _key_to_shader.find( key_in ) ;
                if( iter == _key_to_shader.end() ) return nullptr ;

                return iter->second.compilation_status ;
            }
            
        };
    }
}

#endif
