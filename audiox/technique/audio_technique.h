//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIOX_TECHNIQUE_AUDIO_TECHNIQUE_H_
#define _SNAKEOIL_AUDIOX_TECHNIQUE_AUDIO_TECHNIQUE_H_

#include "../api.h"
#include "../protos.h"
#include "../typedefs.h"
#include "../result.h"

#include "api_id.h"
#include "technique_state.h"
#include "../plug/iplug.h"

#include <snakeoil/audio/protos.h>
#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/container/vector.hpp>

namespace so_audiox
{
    class SNAKEOIL_AUDIOX_API audio_technique
    {
        so_this_typedefs( audio_technique ) ;

        friend class audio_system ;

        struct plug_data
        {
            so_this_typedefs( plug_data ) ;

            plug_data( void_t ){}
            plug_data( this_cref_t ) = delete ;
            plug_data( this_rref_t rhv )
            {
                aid = rhv.aid ;
                so_move_member_ptr( api_ptr, rhv ) ;
                so_move_member_ptr( plug_ptr, rhv ) ;
                ts = rhv.ts ;
                ts_update = rhv.ts_update ;

                rhv.ts = technique_state::raw ;
                init_flag = rhv.init_flag ;
                rhv.init_flag = false ;
            }
            ~plug_data( void_t ) {}

            so_audiox::api_id_t aid ;
            so_audio::iapi_ptr_t api_ptr ;
            so_audiox::iplug_ptr_t plug_ptr ;
            so_audiox::technique_state ts ;
            so_audiox::technique_state ts_update ;
            
            so_thread::mutex_t mtx_init ;
            so_thread::condition_variable_t cv_init ;
            bool_t init_flag = false ;

        };
        so_typedef( plug_data ) ;
        so_typedefs( so_std::vector< plug_data_ptr_t >, datas ) ;

    private:

        so_audiox::iplug_factory_ptr_t _fac_ptr ;

        so_thread::mutex_t _mtx_plugs ;
        datas_t _datas ;

    public:

        audio_technique( so_audiox::iplug_factory_ptr_t ) ;
        audio_technique( this_cref_t ) = delete ;
        audio_technique( this_rref_t ) ;
        ~audio_technique( void_t ) ; 

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    private: // audio system

        bool_t wait_for_init( void_t ) ;

        so_audiox::technique_state part_00( technique_state target, so_audiox::api_id_t, so_audio::iapi_ptr_t,
            so_audiox::iglobal_service_ptr_t, so_audiox::ilocal_service_ptr_t ) ;

        so_audiox::technique_state part_01_render( so_audiox::iplug::execute_info_cref_t ) ;
        so_audiox::technique_state part_01_render_end( void_t ) ;

        so_audiox::result part_01_update( void_t ) ;

        so_audiox::api_id_t get_used_api( void_t ) const ;

    };
    so_typedef( audio_technique ) ;
}

#endif