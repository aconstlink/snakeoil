//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "stb_module.h"

#include "../../enums/image_file_formats.h"
#include "../../image/deduce_image_format.h"
#include "../../image/image.h"

#include "../../manager/image_manager.h"

#include <snakeoil/std/filesystem/filesystem.hpp>

#include <snakeoil/thread/task/tasks.h>
#include <snakeoil/memory/guards/malloc_guard.hpp>
#include <snakeoil/log/log.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

using namespace so_imex ;

//*************************************************************************************
so_thread::task_graph_t stb_module::export_image( export_params_cref_t params_in ) 
{
    if( so_log::log::error( so_core::is_nullptr(params_in.img_mgr_ptr), 
        "[so_imex::stb_module::export_image] : invalid manager pointer" ) )
    {
        so_imex::sync_object::set_not_signal( params_in.sync_ptr, so_imex::invalid_argument ) ;
        return so_thread::task_graph_t() ;
    }    

    so_thread::void_funk_task_ptr_t task_ptr = so_thread::void_funk_task_t::create( 
        [params_in, this]( void_t )
    {
        so_imex::image_file_format iff = params_in.file_format ;
        if(iff == so_imex::image_file_format::undefined)
            iff = so_imex::deduce_image_format::from_path( params_in.path_to_file ) ;

        if(so_log::log::error( iff == so_imex::image_file_format::undefined,
            "[so_imex::stb_module::export_image] : undefined image file format" ))
        {
            return so_imex::sync_object::set_and_signal( params_in.sync_ptr, 
                so_imex::extension_not_supported ) ;
        }
        
        so_imex::image_manager_t::handle_t hnd ;
        {
            auto const res = params_in.img_mgr_ptr->acquire( params_in.image_key, 
                "[so_imex::stb_module::export_image] : exporting the image", hnd ) ;

            if( so_core::is_not(res) )
            {
                return so_log::log::error("[so_imex::stb_module::export_image] : \
                                           image does not exist : " + params_in.image_key ) ;
            }
        }

        so_imex::iimage_ptr_t iimg_ptr = hnd->data_ptr ;

        so_imex::image_ptr_t img_ptr = dynamic_cast<so_imex::image_ptr_t>(iimg_ptr) ;
        if( so_log::log::error( so_core::is_nullptr(img_ptr), 
            "[so_imex::stb_module::export_image] : can not cast image" ) )
        {
            return so_imex::sync_object::set_and_signal( params_in.sync_ptr, 
                so_imex::invalid_cast ) ;
        }

        if( so_log::log::error( so_core::is_nullptr(img_ptr->data_ptr),
            "[so_imex::stb_module::export_image] : can not cast image" ) )
        {
            return so_imex::sync_object::set_and_signal( params_in.sync_ptr,
                    so_imex::failed ) ;
        }
        
        int_t const comp = int_t( so_imex::to_component_count(img_ptr->pix_format) ) ;

        int write_res = 0 ;

        int_t const width = int_t(img_ptr->width) ;
        int_t const height = int_t(img_ptr->height) ;
        void_cptr_t data_ptr = img_ptr->data_ptr ;
        
        // check if path exists to see if it must be created
        {
            so_io::path_t const path = params_in.path_to_file ;
            
            if( so_core::is_not(so_std::filesystem::exists( path.parent_path() )) )
            {
                so_std::filesystem::create_directories( path.parent_path() ) ;
            }
        }

        // stb seems to be unable to handle '\\' in the path string
        so_std::string spath = params_in.path_to_file.string() ;
        {
            std::replace( spath.begin(), spath.end(), '\\', '/' ) ;
        }
        char_cptr_t path_ptr = spath.c_str() ;

        switch(iff)
        {
            case so_imex::image_file_format::png:
            {
                so_memory::malloc_guard<byte_t> tmp_dest( width * height * comp ) ;
                this_t::flip_vertically( tmp_dest, (byte_ptr_t)data_ptr, width, height, comp ) ;
                write_res = stbi_write_png( path_ptr, width, height, comp, tmp_dest, 0 ) ;
            }
            break ;

            case so_imex::image_file_format::hdr:
            write_res = stbi_write_hdr( path_ptr, width, height, comp, float_cptr_t(data_ptr) ) ;
            break ;

            case so_imex::image_file_format::tga:
            write_res = stbi_write_tga( path_ptr, width, height, comp, data_ptr ) ;
            break ;

            case so_imex::image_file_format::bmp:
            write_res = stbi_write_bmp( path_ptr, width, height, comp, data_ptr ) ;
            break ;

            default:
            so_log::log::error( "[stb_module::export_image] : file format type not supported or missing." ) ;
            so_imex::sync_object::set_and_signal( params_in.sync_ptr,
                so_imex::extension_not_supported ) ;
            return ;
        }

        if( so_log::log::error( write_res == 0, 
            "[so_imex::stb_module::export_image] : stb write failed" ) )
        {
            return so_imex::sync_object::set_and_signal( params_in.sync_ptr,
                    so_imex::failed ) ;
        }

        return so_imex::sync_object::set_and_signal( params_in.sync_ptr,
                    so_imex::ok ) ;
    } ) ;

    

    return so_thread::task_graph_t(task_ptr) ;
}
