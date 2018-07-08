//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gl33_resource_bridge_plug.h"
#include "gfx_config.h"

#include <snakeoil/imex/global.h>
#include <snakeoil/imex/image/image.h>
#include <snakeoil/imex/manager/image_manager.h>
#include <snakeoil/imex/manager/manager_registry.h>
#include <snakeoil/imex/module/module_registry.h>

#include <snakeoil/gpu/api/gl/igl_33_api.h>
#include <snakeoil/gpu/image/color_image_2d.hpp>
#include <snakeoil/gpu/texture/texture_2d.h>
#include <snakeoil/gpu/manager/gpu_manager.h>
#include <snakeoil/gpu/image/image_creator.h>

#include <snakeoil/io/global.h>
#include <snakeoil/log/global.h>

using namespace so_gfx ;
using namespace so_gfx::so_gl33 ;

//*************************************************************************************
gl33_resource_bridge_plug::gl33_resource_bridge_plug( resource_bridge_t::shared_data_ptr_t sd_ptr,
    so_gpu::so_gl::igl_33_api_ptr_t aptr ) : this_base_t( aptr )
{
    _sd_ptr = sd_ptr ;
}

//*************************************************************************************
gl33_resource_bridge_plug::gl33_resource_bridge_plug( this_rref_t rhv ) :
    this_base_t( std::move( rhv ) )
{
    so_move_member_ptr( _sd_ptr, rhv ) ;
}

//*************************************************************************************
gl33_resource_bridge_plug::~gl33_resource_bridge_plug( void_t )
{
}

//*************************************************************************************
gl33_resource_bridge_plug::this_ptr_t gl33_resource_bridge_plug::create( 
    this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t gl33_resource_bridge_plug::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_gpx::plug_result gl33_resource_bridge_plug::on_load( load_info_cref_t li )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_resource_bridge_plug::on_unload( void_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_resource_bridge_plug::on_initialize( init_info_cref_t ii )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_resource_bridge_plug::on_release( void_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_resource_bridge_plug::on_execute( execute_info_cref_t ei )
{
    resource_bridge_t::shared_data_t::data_items_t dis ;

    {
        so_thread::lock_guard_t lk( _sd_ptr->mtx ) ;
        dis = std::move( _sd_ptr->dis ) ;
    }

    for( auto const & di : dis )
    {
        switch( di.m )
        {
        case resource_bridge_t::shared_data_t::mode::com_upload_image:
            this_t::upload_image( di.name_a, di.name_b, ei ) ;
            break ;

        default:

            break ;
        
        }
    }
    
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
so_gpx::plug_result gl33_resource_bridge_plug::on_update( update_info_cref_t )
{
    return so_gpx::plug_result::ok ;
}

//*************************************************************************************
void_t gl33_resource_bridge_plug::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
void_t so_gfx::so_gl33::gl33_resource_bridge_plug::upload_image( 
    so_std::string_cref_t imex_name, so_std::string_cref_t gpu_name, execute_info_cref_t ei )
{
    // 1. get image from imex and put it into the gpu manager
    {
    
        so_imex::image_manager_t::handle_t hnd ;
        {
            auto const res = so_imex::global_t::manager_registry()->get_image_manager()->acquire( imex_name,
                "[so_gl33::gl33_resource_bridge_plug::upload_image]", hnd ) ;

            if( so_core::is_not( res ) )
            {
                so_log::global_t::warning( "[so_gl33::gl33_resource_bridge_plug::upload_image] : "
                "imex name image does not exist" ) ;
                return ;
            }
        }

        if( so_core::is_nullptr( dynamic_cast<so_imex::image_ptr_t>(hnd->data_ptr) ) )
        {
            so_log::global_t::error("[gl33_resource_bridge_plug::upload_image] : "
                "Can not cast to image. No conversion possible at the moment. The image "
                "will not be uploaded.") ;
            return ;
        }
    
        {
            auto const res = ei.mgr->get_img2d_mgr()->reserve( gpu_name ) ;
        
            // the image was reserved and did not exist before.
            if( res == so_resource::reserved )
            {
                so_imex::image_ptr_t img = dynamic_cast<so_imex::image_ptr_t>(hnd->data_ptr) ;
                so_gpu::image_creator_t ic( img, "[gl33_resource_bridge_plug::upload_image]" ) ;

                auto * gpu_img = ic.move_image_2d() ;
                so_gpu::iimage_2d_ptr_t old_img = nullptr ;

                auto const ires = ei.mgr->get_img2d_mgr()->exchange( gpu_name, gpu_img, old_img ) ;
                if( ires!= so_resource::ok )
                {
                    // race condition? This should not 
                    if( so_core::is_not_nullptr( old_img ) ) old_img->destroy() ;
                }

                this_t::api()->create_image( gpu_img ) ;
            }
            else if( res == so_resource::found )
            {
                // the image can be updated with the new data.
            }
        }

        
    }

    so_gpu::gpu_manager_t::img2d_manager_t::handle_t hnd ;

    // 2. upload image
    {
        auto const res = ei.mgr->get_img2d_mgr()->acquire( gpu_name, "", hnd ) ;
        if( so_core::is_not( res ) )
        {
            so_log::global_t::error("[gl33_resource_bridge_plug::upload_image] : "
                "The image <"+ gpu_name +"> should be there. It was uploaded" ) ;
            return ;
        }
        this_t::api()->alloc_image_memory( hnd.get_ptr(), so_gpu::image_alloc_info_t( true ) ) ; 
    }

    // 2. create texture
    {
        so_gpu::texture_2d_ptr_t tx2d_ptr = so_gpu::texture_2d_t::create(
            so_gpu::texture_2d_t( hnd.get_ptr() ), "[gl33_resource_bridge_plug::upload_image]" ) ;
        
        this_t::api()->create_texture( tx2d_ptr ) ;

        {
            auto const res = ei.mgr->get_tx2d_mgr()->reserve( gpu_name ) ;

            if( res == so_resource::reserved )
            {
                so_gpu::texture_2d_ptr_t old_ptr = nullptr ;

                ei.mgr->get_tx2d_mgr()->exchange( gpu_name, tx2d_ptr, old_ptr ) ;

                so_log::global_t::error( so_core::is_not_nullptr(old_ptr), 
                    "[gl33_resource_bridge_plug::upload_image] : This will leak. The old ptr should be"
                    " nullptr, because the texture was reserved." ) ;
            }
            else if( res == so_resource::found )
            {
                // the image can be updated with the new data. 
                // Use the same texture, just exchange the image
            }
        }
    }

    return void_t();
}
