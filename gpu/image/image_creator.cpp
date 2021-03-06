//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "image_creator.h"

#include "color_image_2d.hpp"

#include <snakeoil/imex/image/image.h>

#include <snakeoil/thread/parallel_for.hpp>

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>
#include <snakeoil/math/utility/index_1d_to_2d.hpp>

#include <snakeoil/log/global.h>
#include <snakeoil/core/macros/move.h>

using namespace so_gpu ;

//*************************************************************************************
image_creator::image_creator( void_t ) 
{}

//*************************************************************************************
image_creator::image_creator( so_imex::image_cptr_t img_ptr, so_memory::purpose_cref_t p )
{
    this_t::construct_from( img_ptr, p ) ;
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
void_t image_creator::construct_from( so_imex::iimage_cptr_t img_ptr, so_memory::purpose_cref_t p )
{
    this_t::construct_from( dynamic_cast< so_imex::image_cptr_t >( img_ptr ), p ) ;
}

//*************************************************************************************
void_t image_creator::construct_from( so_imex::image_cptr_t imex_ptr, so_memory::purpose_cref_t p )
{
    if( so_core::is_nullptr( imex_ptr ) )
        return ;

    if( imex_ptr->data_type == so_core::type::tuchar )
    {
        size_t const num_comp = so_imex::to_component_count( imex_ptr->pix_format ) ;
        if( num_comp == 1 )
        {
            uchar_cptr_t img_data_in_ptr = imex_ptr->data_as<uchar_t>() ;

            typedef uchar_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = image_2d_t::create( p ) ;

            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&] ( so_thread::range_1d< size_t > const & r )
            {
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i = r.begin(); i < r.end(); ++i )
                {
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;
                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ i + 0 ] ) ) ;
                }
            } ) ;

            _img_ptr = gpu_img_ptr ;
        }
        else if( num_comp == 2 )
        {
            uchar_cptr_t img_data_in_ptr = imex_ptr->data_as<uchar_t>() ;

            typedef so_math::vec2uc_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = image_2d_t::create( p ) ;

            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&] ( so_thread::range_1d< size_t > const & r )
            {
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i = r.begin(); i < r.end(); ++i )
                {
                    size_t const index = i * num_comp ;
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;

                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ index + 0 ],
                        img_data_in_ptr[ index + 1 ] ) ) ;
                }
            } ) ;

            _img_ptr = gpu_img_ptr ;
        }
        else if( num_comp == 3 )
        {
            uchar_cptr_t img_data_in_ptr = imex_ptr->data_as<uchar_t>() ;

            typedef so_math::vec4uc_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = image_2d_t::create( p ) ;

            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&] ( so_thread::range_1d< size_t > const & r )
            {
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i = r.begin(); i < r.end(); ++i )
                {
                    size_t const index = i * num_comp ;
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;

                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ index + 0 ],
                        img_data_in_ptr[ index + 1 ],
                        img_data_in_ptr[ index + 2 ],
                        255 ) ) ;
                }
            } ) ;

            _img_ptr = gpu_img_ptr ;
        }
        else if( num_comp == 4 )
        {
            uchar_cptr_t img_data_in_ptr = imex_ptr->data_as<uchar_t>() ;

            typedef so_math::vec4uc_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = image_2d_t::create( p ) ;
            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&] ( so_thread::range_1d< size_t > const & r )
            {
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i = r.begin(); i < r.end(); ++i )
                {
                    size_t const index = i * num_comp ;
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;

                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ index + 0 ],
                        img_data_in_ptr[ index + 1 ],
                        img_data_in_ptr[ index + 2 ],
                        img_data_in_ptr[ index + 3 ] ) ) ;
                }
            } ) ;

            _img_ptr = gpu_img_ptr ;
        }
        else
        {
            so_log::global::warning( p + " : num components not available in test" ) ;
        }
    }
    else if( imex_ptr->data_type == so_core::type::tfloat )
    {
        size_t const num_comp = so_imex::to_component_count( imex_ptr->pix_format ) ;
        if( num_comp == 1 )
        {
            float_cptr_t img_data_in_ptr = imex_ptr->data_as<float_t>() ;

            typedef float_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = image_2d_t::create( p ) ;

            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&] ( so_thread::range_1d< size_t > const & r )
            {
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i = r.begin(); i < r.end(); ++i )
                {
                    size_t const index = i * num_comp ;
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;

                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ index + 0 ] ) ) ;
                }
            } ) ;

            _img_ptr = gpu_img_ptr ;
        }
        // @todo comp == 2
        else if( num_comp == 3 )
        {
            float_cptr_t img_data_in_ptr = imex_ptr->data_as<float_t>() ;

            typedef so_math::vec4f_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = image_2d_t::create( p ) ;

            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&] ( so_thread::range_1d< size_t > const & r )
            {
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i = r.begin(); i < r.end(); ++i )
                {
                    size_t const index = i * num_comp ;
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;

                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ index + 0 ],
                        img_data_in_ptr[ index + 1 ],
                        img_data_in_ptr[ index + 2 ],
                        1.0f ) ) ;
                }
            } ) ;

            _img_ptr = gpu_img_ptr ;
        }
        else if( num_comp == 4 )
        {
            float_cptr_t img_data_in_ptr = imex_ptr->data_as<float_t>() ;

            typedef so_math::vec4f_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = image_2d_t::create( p ) ;
            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&] ( so_thread::range_1d< size_t > const & r )
            {
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i = r.begin(); i < r.end(); ++i )
                {
                    size_t const index = i * num_comp ;
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;

                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ index + 0 ],
                        img_data_in_ptr[ index + 1 ],
                        img_data_in_ptr[ index + 2 ],
                        img_data_in_ptr[ index + 3 ] ) ) ;
                }
            } ) ;

            _img_ptr = gpu_img_ptr ;
        }
    }
}

//*************************************************************************************
void_t image_creator::copy_from( so_imex::iimage_cptr_t img_in, so_gpu::iimage_2d_ptr_t img_ptr_to )
{
    auto * imex_ptr = dynamic_cast< so_imex::image_cptr_t >( img_in ) ;
    if( so_core::is_nullptr( imex_ptr ) )
        return ;

    if( imex_ptr->data_type == so_core::type::tuchar )
    {
        size_t const num_comp = so_imex::to_component_count( imex_ptr->pix_format ) ;
        if( num_comp == 1 )
        {
            uchar_cptr_t img_data_in_ptr = imex_ptr->data_as<uchar_t>() ;

            typedef uchar_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = dynamic_cast< image_2d_ptr_t >( img_ptr_to ) ;
            if( so_core::is_nullptr( gpu_img_ptr ) )
            {
                return ;
            }

            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&]( so_thread::range_1d< size_t > const & r ) 
            { 
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i=r.begin(); i<r.end(); ++i )
                {
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;
                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ i + 0 ] ) ) ;
                }
            } ) ;
        }
        else if( num_comp == 2 )
        {
            uchar_cptr_t img_data_in_ptr = imex_ptr->data_as<uchar_t>() ;

            typedef so_math::vec2uc_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = dynamic_cast< image_2d_ptr_t >( img_ptr_to ) ;
            if( so_core::is_nullptr( gpu_img_ptr ) )
            {
                return ;
            }

            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&] ( so_thread::range_1d< size_t > const & r )
            {
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i = r.begin(); i < r.end(); ++i )
                {
                    size_t const index = i * num_comp ;
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;

                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ index + 0 ],
                        img_data_in_ptr[ index + 1 ] ) ) ;
                }
            } ) ;
        }
        else if( num_comp == 3 )
        {
            uchar_cptr_t img_data_in_ptr = imex_ptr->data_as<uchar_t>() ;

            typedef so_math::vec4uc_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = dynamic_cast< image_2d_ptr_t >( img_ptr_to ) ;
            if( so_core::is_nullptr( gpu_img_ptr ) )
            {
                return ;
            }

            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&] ( so_thread::range_1d< size_t > const & r )
            {
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i = r.begin(); i < r.end(); ++i )
                {
                    size_t const index = i * num_comp ;
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;

                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ index + 0 ],
                        img_data_in_ptr[ index + 1 ],
                        img_data_in_ptr[ index + 2 ],
                        255 ) ) ;
                }
            } ) ;
        }
        else if( num_comp == 4 )
        {
            uchar_cptr_t img_data_in_ptr = imex_ptr->data_as<uchar_t>() ;

            typedef so_math::vec4uc_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = dynamic_cast< image_2d_ptr_t >( img_ptr_to ) ;
            if( so_core::is_nullptr( gpu_img_ptr ) )
            {
                return ;
            }
            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&] ( so_thread::range_1d< size_t > const & r )
            {
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i = r.begin(); i < r.end(); ++i )
                {
                    size_t const index = i * num_comp ;
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;

                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ index + 0 ],
                        img_data_in_ptr[ index + 1 ],
                        img_data_in_ptr[ index + 2 ],
                        img_data_in_ptr[ index + 3 ] ) ) ;
                }
            } ) ;
        }
        else
        {
            so_log::global::warning( " : num components not available in test" ) ;
        }
    }
    else if( imex_ptr->data_type == so_core::type::tfloat )
    {
        size_t const num_comp = so_imex::to_component_count( imex_ptr->pix_format ) ;
        if( num_comp == 1 )
        {
            float_cptr_t img_data_in_ptr = imex_ptr->data_as<float_t>() ;

            typedef float_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = dynamic_cast< image_2d_ptr_t >( img_ptr_to ) ;
            if( so_core::is_nullptr( gpu_img_ptr ) )
            {
                return ;
            }

            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&] ( so_thread::range_1d< size_t > const & r )
            {
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i = r.begin(); i < r.end(); ++i )
                {
                    size_t const index = i * num_comp ;
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;

                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ index + 0 ] ) ) ;
                }
            } ) ;
        }
        // @todo comp == 2
        else if( num_comp == 3 )
        {
            float_cptr_t img_data_in_ptr = imex_ptr->data_as<float_t>() ;

            typedef so_math::vec4f_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = dynamic_cast< image_2d_ptr_t >( img_ptr_to ) ;
            if( so_core::is_nullptr( gpu_img_ptr ) )
            {
                return ;
            }

            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&] ( so_thread::range_1d< size_t > const & r )
            {
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i = r.begin(); i < r.end(); ++i )
                {
                    size_t const index = i * num_comp ;
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;

                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ index + 0 ],
                        img_data_in_ptr[ index + 1 ],
                        img_data_in_ptr[ index + 2 ],
                        1.0f ) ) ;
                }
            } ) ;
        }
        else if( num_comp == 4 )
        {
            float_cptr_t img_data_in_ptr = imex_ptr->data_as<float_t>() ;

            typedef so_math::vec4f_t pixel_t ;
            typedef so_gpu::color_image_2d<pixel_t> _image_2d_t ;
            so_typedefs( _image_2d_t, image_2d ) ;

            image_2d_ptr_t gpu_img_ptr = dynamic_cast< image_2d_ptr_t >( img_ptr_to ) ;
            if( so_core::is_nullptr( gpu_img_ptr ) )
            {
                return ;
            }
            gpu_img_ptr->resize( imex_ptr->width, imex_ptr->height ) ;

            so_thread::parallel_for<size_t>( so_thread::range_1d<size_t>( 0, imex_ptr->width*imex_ptr->height ),
                [&] ( so_thread::range_1d< size_t > const & r )
            {
                so_math::index_1d_to_2d<size_t> i1dt2d( imex_ptr->width, imex_ptr->height ) ;

                for( size_t i = r.begin(); i < r.end(); ++i )
                {
                    size_t const index = i * num_comp ;
                    so_math::vector2<size_t> const xy = i1dt2d.to_index( i ) ;

                    gpu_img_ptr->set_pixel( xy.x(), xy.y(), pixel_t(
                        img_data_in_ptr[ index + 0 ],
                        img_data_in_ptr[ index + 1 ],
                        img_data_in_ptr[ index + 2 ],
                        img_data_in_ptr[ index + 3 ] ) ) ;
                }
            } ) ;
        }
    }
}