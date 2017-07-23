//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_IMAGE_IMAGE_H_
#define _SNAKEOIL_GPU_IMAGE_IMAGE_H_

#include "../object.h"

#include "../enums/image_type.h"
#include "../enums/image_formats.h"
#include "../enums/pixel_type.h"

namespace so_gpu
{
    class SNAKEOIL_GPU_API image : public so_gpu::object
    {
        so_this_typedefs( image ) ;

    private:

        image_type _image_type = image_type::undefined ;
        image_format _image_format = image_format::undefined ;
        pixel_type _pixel_type = pixel_type::undefined ;

    public:

        image( void_t ) ;
        image( this_rref_t rhv ) ;
        virtual ~image( void_t ) ;

    public:

        void_t set_type( image_type it ) ;
        image_type get_type( void_t ) const ;

        void_t set_format( image_format imgf ) ;
        image_format get_format( void_t ) const ;
        
        pixel_type get_pixel_type( void_t ) const ;
        void_t set_pixel_type( pixel_type ) ;

    public: // interface

        virtual size_t get_sib( void_t ) const = 0 ;
        virtual void_ptr_t get_raw_pointer( void_t ) = 0 ;
        virtual void_cptr_t get_raw_pointer( void_t ) const = 0 ;
    };
    so_typedef( image ) ;
}

#endif

