//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_IMAGERY_IMAGE_CREATOR_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_IMAGERY_IMAGE_CREATOR_H_

#include "../../typedefs.h"
#include "../../result.h"
#include "../../api.h"
#include "../../protos.h"

#include <snakeoil/imex/protos.h>

#include <snakeoil/gpu/result.h>
#include <snakeoil/gpu/protos.h>


namespace so_manager
{
    namespace so_graphics
    {
        class SNAKEOIL_MANAGER_API image_creator
        {
            so_this_typedefs( image_creator ) ;

        private:

            so_gpu::image_ptr_t _img_ptr = nullptr ;

        private:

            image_creator( this_cref_t ){}

        public:

            image_creator( void_t ) ;
            image_creator( so_imex::image_ptr_t ) ;
            image_creator( this_rref_t ) ;
            virtual ~image_creator( void_t ) ;

        public:

            so_gpu::iimage_2d_ptr_t move_image_2d( void_t ) ;

        public:

            void_t construct_from( so_imex::image_ptr_t ) ;
        };
    }
}

#endif