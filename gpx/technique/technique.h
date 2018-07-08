//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPX_TECHNIQUE_TECHNIQUE_H_
#define _SNAKEOIL_GPX_TECHNIQUE_TECHNIQUE_H_

#include "itechnique.h"
#include "technique_state.h"
#include "../window/window_id.h"

#include "../system/render_system_structs.h"
#include <snakeoil/thread/mutex.h>

namespace so_gpx
{
    class SNAKEOIL_GPX_API technique
    {
        so_this_typedefs( technique ) ;

        friend class render_system ;

        struct plug_data
        {
            so_this_typedefs( plug_data ) ;

            plug_data( void_t ) {}
            plug_data( this_cref_t ) = delete ;
            plug_data( this_rref_t rhv )
            {
                wid = rhv.wid ;
                so_move_member_ptr( api_ptr, rhv ) ;
                so_move_member_ptr( plug_ptr, rhv ) ;
                
                ts = rhv.ts ;
                rs = rhv.rs ;
                sg = rhv.sg ;

                scheduled = rhv.scheduled ;
                rhv.scheduled = false ;
                rnd_id = rhv.rnd_id ;
                rhv.rnd_id = size_t(0) ;
                rnd_max = rhv.rnd_max ;
                rhv.rnd_max = size_t(-1) ;
                updated = rhv.updated ;
                rhv.updated = false ;

            }
            ~plug_data( void_t ) {}

            so_gpx::window_id_t wid ;
            
            so_gpx::technique_rest_state rs = so_gpx::technique_rest_state::offline ;
            so_gpx::technique_transition_state ts = so_gpx::technique_transition_state::none ;
            so_gpx::technique_schedule_goal sg = so_gpx::technique_schedule_goal::none ;

            so_gpu::iapi_ptr_t api_ptr ;
            so_gpx::iplug_ptr_t plug_ptr ;
            
            size_t rnd_id = size_t(0) ;
            size_t rnd_max = size_t(-1) ;

            so_thread::mutex_t mtx_s ;
            bool_t scheduled = false ;

            bool_t updated = false ;

        public: // 

            so_thread::mutex_t mtx_init ;
            so_thread::condition_variable_t cv_init ;
            bool_t init_flag = false ;
        };
        so_typedef( plug_data ) ;
        so_typedefs( so_std::vector< plug_data_ptr_t >, plug_datas ) ;

    private:

        so_gpx::iplug_factory_ptr_t _plug_fac_ptr ;


        so_thread::mutex_t _mtx_plugs ;
        plug_datas_t _plugs ;

    public:

        technique( so_gpx::iplug_factory_ptr_t ) ;
        technique( this_cref_t ) = delete ;
        technique( this_rref_t ) ;
        ~technique( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    
    private: // render system

        bool_t wait_for_init( so_gpx::window_id_t ) ;
        
        so_gpx::technique_transition_result part_00( technique_schedule_goal target,
            so_gpx::window_id_t, so_gpu::iapi_ptr_t,
            so_gpx::iglobal_service_ptr_t, so_gpx::ilocal_service_ptr_t, so_gpu::gpu_manager_ptr_t ) ;

        so_gpx::technique_transition_result check_part_00_finished( technique_schedule_goal target, 
            so_gpx::window_id_t ) ;

        void_t part_01_render( so_gpx::window_id_t, so_gpx::schedule_instance_cref_t,
            so_gpu::iapi_ptr_t, so_gpu::gpu_manager_ptr_t ) ;
        void_t part_01_render_end( so_gpx::window_id_t, so_gpx::schedule_instance_cref_t ) ;


        void_t part_01_render_pseudo( so_gpx::window_id_t ) ;

        so_gpx::result part_01_update( so_gpx::window_id_t ) ;
        so_gpx::result part_01_update_end( so_gpx::window_id_t ) ;
        
    };
    so_typedef( technique ) ;
}

#endif