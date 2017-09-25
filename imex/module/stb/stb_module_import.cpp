//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "stb_module.h"

#include "../../image/image.h"
#include "../../image/deduce_image_format.h"
#include "../../manager/image_manager.h"
#include "../../manager/manager_registry.h"

#include "../../global.h"

#include <snakeoil/io/global.h>
#include <snakeoil/io/system/system.h>

#include <snakeoil/memory/guards/malloc_guard.hpp>
#include <snakeoil/thread/task/tasks.h>

#include <snakeoil/log/global.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace so_imex ;

//*************************************************************************************
so_thread::task_graph_t stb_module::import_image( import_params_cref_t params_in ) 
{
    so_imex::sync_object::set_not_signal( params_in.sync_ptr, so_imex::not_ready ) ;

    so_thread::void_funk_task_ptr_t import_task_ptr = 
        so_thread::void_funk_task_t::create( [=]( void_t )
    {
        so_memory::malloc_guard<char_t> data_buffer ;
        
        // the result
        {
            so_io::load_handle_t lh = so_io::global::load( params_in.path_to_file ) ;
            auto res = lh.wait_for_operation( [&]( char_cptr_t din, size_t sib, so_io::result ires )
            {
                if( so_io::no_success( ires ) ) return ;
                data_buffer = so_memory::malloc_guard<char_t>( din, sib ) ;
            } ) ;

            if( so_log::global::error( so_io::no_success( res ),
                "[so_imex::stb_module::import_image] : load image via stb with : " +
                so_io::to_string( res ) ) )
            {
                return so_imex::sync_object::set_and_signal(
                    params_in.sync_ptr, so_imex::failed ) ;
            }
        }
        
        so_imex::image_file_format imagef = 
            params_in.file_format == so_imex::image_file_format::undefined ?
            so_imex::deduce_image_format::from_path( params_in.path_to_file ) : 
            params_in.file_format ;

        int width, height, comp ;
        
        // loading the image in uchar format
        // @todo float images
        {
            uchar_ptr_t stb_data_ptr = stbi_load_from_memory( uchar_cptr_t(data_buffer.get()), 
                int_t(data_buffer.size()), &width, &height, &comp, 0 ) ;
        
            if( so_log::global::error(stb_data_ptr == nullptr, 
                "[so_imex::stb_module::import_image] : stbi_load_from_memory" ) )
            {
                char_cptr_t stbi_err_msg = stbi_failure_reason() ;
                so_log::global::error( "[so_imex::stb_module::import_image] : stb_image says : " 
                    + std::string(stbi_err_msg) ) ;

                return so_imex::sync_object::set_and_signal( params_in.sync_ptr, so_imex::failed ) ;
            }

            so_imex::image_ptr_t imex_image_ptr = so_imex::image::create( 
                so_imex::image( so_imex::from_component_count(comp), so_core::type::tuchar, 
                    width, height ), "[so_imex::stb_module::import_image] : image" ) ;

            imex_image_ptr->image_fmt = imagef ;

            uchar_ptr_t dest_ptr = imex_image_ptr->data_as<uchar_t>() ;

            int index = 0 ;
            
#if 0
            for( int y=height-1; y>=0; --y )
            {
                for( int x=0; x<width; ++x )
                {
                    size_t const src_index = y * width + x ;

                    for( int c=0; c<comp; ++c )
                    {
                        dest_ptr[index++] = stb_data_ptr[src_index*comp+c] ;                        
                    }
                }
            }
#else
            for( int y = 0; y < height; ++y )
            {
                for( int x = 0; x < width; ++x )
                {
                    size_t const src_index = y * width + x ;

                    for( int c = 0; c < comp; ++c )
                    {
                        dest_ptr[index++] = stb_data_ptr[src_index*comp + c] ;
                    }
                }
            }
#endif
            stbi_image_free( stb_data_ptr ) ;

            // store the new image
            {
                auto * img_mgr = params_in.img_mgr_ptr ;

                if( so_core::is_nullptr( img_mgr ) )
                {
                    img_mgr = so_imex::global::manager_registry()->get_image_manager() ;
                }

                so_imex::image_manager_t::manage_params sr ;
                sr.data_ptr = imex_image_ptr ;
                sr.file_path = params_in.path_to_file ;
                
                // @note not 100 % bullet proof. between reserve and exchange, the data
                // could be changed from somewhere else.
                auto const res = img_mgr->reserve_by_key( params_in.key ) ;
                if( res == so_imex::found || res == so_imex::reserved )
                {
                    auto const res2 = img_mgr->exchange( params_in.key, sr ) ;
                    if( so_log::global::error( so_imex::no_success( res2 ),
                        "[so_imex::stb_module::import_image] : manageing image" ) )
                    {
                        so_imex::image::destroy( imex_image_ptr ) ;
                        so_imex::sync_object::set_and_signal( params_in.sync_ptr, so_imex::failed ) ;
                        return ;
                    }
                }
            }
        }

        so_imex::sync_object::set_and_signal( params_in.sync_ptr, so_imex::ok ) ;

    }, "[so_imex::stb_module::import_image] : import task" ) ;

    return so_thread::task_graph( import_task_ptr ) ;
}
