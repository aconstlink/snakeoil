//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_IMAGE_IMAGE_H_
#define _SNAKEOIL_IMEX_IMAGE_IMAGE_H_

#include "iimage.h"

#include "../enums/image_file_formats.h"
#include "../enums/pixel_formats.h"

#include <snakeoil/core/enums/types.h>

namespace so_imex
{
    class SNAKEOIL_IMEX_API image : public iimage
    {
        so_this_typedefs( image ) ;

        typedef size_t width_t ;
        typedef size_t height_t ;

    public:

        so_imex::image_file_format image_fmt = so_imex::image_file_format::undefined ;
        so_imex::pixel_format pix_format = so_imex::pixel_format::undefined ;

        /// tells which data type data_ptr is.
        so_core::type data_type = so_core::type::undefined ;
        
        void_ptr_t data_ptr = nullptr ;
        size_t sib = 0 ;

        size_t width = 0 ;
        size_t height = 0 ;

    private:

        image( this_cref_t ) {}

    public:

        image( void_t ) ;
        image( so_imex::pixel_format, so_core::type, width_t, height_t ) ;
        image( this_rref_t ) ;
        ~image( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t create_image_data( so_imex::pixel_format, so_core::type, width_t, height_t ) ;

        template< typename type_t >
        type_t * data_as( void_t )
        {
            return reinterpret_cast<type_t*>(data_ptr) ;
        }

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( image ) ;
}

#endif
