//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "image_creator.h"

#include "color_image_2d.hpp"

#include <snakeoil/imex/image/image.h>

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>

#include <snakeoil/log/log.h>
#include <snakeoil/core/macros/move.h>

using namespace so_gpu ;

//*************************************************************************************
image_creator::image_creator( void_t ) 
{}

//*************************************************************************************
image_creator::image_creator( so_imex::image_ptr_t img_ptr ) 
{
    this_t::construct_from( img_ptr ) ;
}

//*************************************************************************************
image_creator::image_creator( this_rref_t rhv ) 
{
    so_move_member_ptr( _img_ptr, rhv ) ;
}

//*************************************************************************************
image_creator::~image_creator( void_t ) 
{
    if( _img_ptr != nullptr )
    {
        _img_ptr->destroy() ;
    }
}

//*************************************************************************************
so_gpu::iimage_2d_ptr_t image_creator::move_image_2d( void_t ) 
{
    if( dynamic_cast<so_gpu::iimage_2d_ptr_t>(_img_ptr) != nullptr )
    {
        auto * old_ptr = static_cast<so_gpu::iimage_2d_ptr_t>(_img_ptr) ;
        _img_ptr = nullptr ;

        return old_ptr ;
    }

    return nullptr ;
}

//*************************************************************************************
void_t image_creator::construct_from( so_imex::image_ptr_t imex_ptr ) 
{
    if( imex_ptr->data_type == so_core::type::tuchar )
    {
        size_t const num_comp = so_imex::to_component_count( imex_ptr->pix_format ) ;
        if( num_comp == 1 )
        {
            uchar_ptr_t img_data_in_ptr = imex_ptr->data_as<uchar_t>() ;

            typedef uchar_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = image_2d_t::create(
                "[so_manager::image_creator::construct_from] : gpu image" ) ;

            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            size_t index = 0 ;
            for( size_t y = 0; y < imex_ptr->height; ++y )
            {
                for( size_t x = 0; x < imex_ptr->width; ++x )
                {
                    gpu_img_ptr->set_pixel( x, y, pixel_t(
                        img_data_in_ptr[index + 0] ) ) ;

                    index += num_comp ;
                }
            }

            _img_ptr = gpu_img_ptr ;
        }
        else if( num_comp == 2 )
        {
            uchar_ptr_t img_data_in_ptr = imex_ptr->data_as<uchar_t>() ;

            typedef so_math::vec2uc_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = image_2d_t::create( 
                "[so_manager::image_creator::construct_from] : gpu image" ) ;

            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            size_t index = 0 ;
            for( size_t y=0; y<imex_ptr->height; ++y )
            {
                for( size_t x=0; x<imex_ptr->width; ++x )
                {
                    gpu_img_ptr->set_pixel( x, y, pixel_t(
                        img_data_in_ptr[index+0], 
                        img_data_in_ptr[index+1] ) ) ;

                    index += num_comp ;
                }
            }

            _img_ptr = gpu_img_ptr ;
        }
        else if( num_comp == 3 )
        {
            uchar_ptr_t img_data_in_ptr = imex_ptr->data_as<uchar_t>() ;

            typedef so_math::vec3uc_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = image_2d_t::create( 
                "[so_manager::image_creator::construct_from] : gpu image" ) ;

            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            size_t index = 0 ;
            for( size_t y=0; y<imex_ptr->height; ++y )
            {
                for( size_t x=0; x<imex_ptr->width; ++x )
                {
                    gpu_img_ptr->set_pixel( x, y, pixel_t(
                        img_data_in_ptr[index+0], 
                        img_data_in_ptr[index+1], 
                        img_data_in_ptr[index+2] ) ) ;

                    index += num_comp ;
                }
            }

            _img_ptr = gpu_img_ptr ;
        }
        else if( num_comp == 4 )
        {
            uchar_ptr_t img_data_in_ptr = imex_ptr->data_as<uchar_t>() ;

            typedef so_math::vec4uc_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = image_2d_t::create("[] : gpu image") ;
            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            size_t index = 0 ;
            for( size_t y=0; y<imex_ptr->height; ++y )
            {
                for( size_t x=0; x<imex_ptr->width; ++x )
                {
                    gpu_img_ptr->set_pixel( x, y, pixel_t(
                        img_data_in_ptr[index+0], 
                        img_data_in_ptr[index+1], 
                        img_data_in_ptr[index+2],
                        img_data_in_ptr[index+3]) ) ;

                    index += num_comp ;
                }
            }

            _img_ptr = gpu_img_ptr ;
        }
        else
        {
            so_log::log::warning("num components not available in test") ;
        }
    }
    // else float ... here.
    //
}

//*************************************************************************************