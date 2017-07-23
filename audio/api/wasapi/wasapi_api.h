//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_API_WASAPI_WASAPI_H_
#define _SNAKEOIL_AUDIO_API_WASAPI_WASAPI_H_

#include "iwasapi_api.h"

#include "../../handler/handler.h"
#include "../../handler/handler_id.h"

#include <snakeoil/thread/primitive/sync_object.hpp>
#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/container/vector.hpp>



namespace so_audio
{
    namespace so_win32
    {
        class SNAKEOIL_AUDIO_API wasapi_api : public iwasapi_api
        {
            so_this_typedefs( wasapi_api ) ;

        private: // loopback

            /// old
            struct loopback_data
            {
                so_this_typedefs( loopback_data ) ;

                loopback_data( void_t ) {}
                loopback_data( this_cref_t ) = delete ;
                loopback_data( this_rref_t rhv )
                {
                    _lbt = std::move( rhv._lbt ) ;
                    is_done = rhv.is_done ;
                    so_init = std::move( rhv.so_init ) ;
                    handler_added = rhv.handler_added ;
                    hnds = std::move( rhv.hnds );

                }
                ~loopback_data( void_t ) {}

                std::thread _lbt ;

                so_thread::condition_variable_t cv_ready ;
                so_thread::mutex_t mtx_ready ;
                bool_t is_done = false ;

                so_thread::sync_object_t so_init ;

                so_thread::mutex_t mtx_handlers ;
                so_thread::condition_variable_t cv_handlers ;
                bool_t handler_added = false ;
                so_typedefs( so_std::vector< so_audio::loopback_handler_t >, handlers ) ;
                handlers_t hnds ;
            };
            so_typedef( loopback_data ) ;
            loopback_data_ptr_t _lbptr = nullptr ;


            so_struct_proto_typedefs( loopback_device ) ;

        private:

            
            loopback_device_ptr_t _lp_dev_ptr = nullptr ;

        public:

            wasapi_api( void_t ) ;
            wasapi_api( this_cref_t ) = delete ;
            wasapi_api( this_rref_t ) ;
            virtual ~wasapi_api( void_t ) ;


        public:

            virtual bool_t install_handler( so_audio::loopback_handler_t, handler_id_out_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public: // iwasapi interface

            virtual so_audio::result create( loopback_buffer_ptr_t ) ;
            virtual so_audio::result release( loopback_buffer_ptr_t ) ;
            virtual so_audio::result store( loopback_buffer_ptr_t ) ;

        public: // iapi interface

            virtual bool_t initialize( void_t ) ;
            virtual void_t release( void_t ) ;

            virtual api_type get_type( void_t ) const ;
            virtual void_t destroy( void_t ) ;

        };
        so_typedef( wasapi_api ) ;
    }
}

#endif