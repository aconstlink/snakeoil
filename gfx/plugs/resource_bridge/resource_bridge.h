#pragma once

#include "../../api.h"
#include "../../protos.h"
#include "../../typedefs.h"

#include <snakeoil/gpx/protos.h>
#include <snakeoil/gpx/technique/technique_id.h>
#include <snakeoil/std/container/vector.hpp>

namespace so_gfx
{
    class SNAKEOIL_GFX_API resource_bridge
    {
        so_this_typedefs( resource_bridge ) ;

    public:

        struct shared_data
        {
            so_this_typedefs( shared_data ) ;

            enum class mode
            {
                invalid,
                com_upload_image
            };

            struct data_item
            {
                mode m ;
                so_std::string_t name_a ;
                so_std::string_t name_b ;
            };
            so_typedef( data_item ) ;
            

            so_typedefs( so_std::vector< data_item_t >, data_items ) ;
            data_items_t dis ;

            so_thread::mutex_t mtx ;

            shared_data( void_t ) {}
            shared_data( this_cref_t ) = delete ;
            shared_data( this_rref_t rhv )
            {
                *this = std::move( rhv ) ;
            }
            ~shared_data( void_t ) {}
            this_ref_t operator = ( this_cref_t ) = delete ;
            this_ref_t operator = ( this_rref_t rhv )
            {
                dis = std::move( rhv.dis ) ;
                return *this ;
            }
        };
        so_typedef( shared_data ) ;

    private:

        shared_data_ptr_t _sd = nullptr ;

        so_gfx::resource_bridge_plug_factory_ptr_t _fac_ptr = nullptr ;
        so_gpx::technique_id_t _tid ;
        so_gpx::render_system_ptr_t _rs ;

    public:

        resource_bridge( so_gpx::render_system_ptr_t ) ;
        resource_bridge( this_cref_t ) = delete ;
        resource_bridge( this_rref_t ) ;
        ~resource_bridge( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        // upload image
        void_t upload_image( so_std::string_cref_t imex_name, so_std::string_cref_t gpu_name ) ;

        // download image
    } ;
    so_typedef( resource_bridge ) ;
}