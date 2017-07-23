//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_DRIVER_INFO_STRUCTS_H_
#define _SNAKEOIL_GPU_DRIVER_INFO_STRUCTS_H_

#include "../typedefs.h"
#include "../protos.h"

namespace so_gpu
{
    struct image_alloc_info
    {
        image_alloc_info( bool_t do_copy_ ) : do_copy(do_copy_) {}

        bool_t do_copy = false ;
    };
    so_typedef( image_alloc_info ) ;

    struct image_save_info
    {};
    so_typedef( image_save_info ) ;
        
    struct image_load_info
    {
    };
    so_typedef( image_load_info ) ;

    struct memory_alloc_info
    {
        memory_alloc_info( void_t ){}
        memory_alloc_info( bool_t do_copy_, size_t sib_=size_t(-1) )
        {
            do_copy = do_copy_ ;
            sib = sib_ ;
        }

        /// copy the buffers contents after allocation?
        bool_t do_copy = false ;
        /// -1: alloc as much as the buffer requires.
        size_t sib = size_t(-1) ;
    };
    so_typedef( memory_alloc_info ) ;
        
    struct memory_transfer_info
    {
        size_t offset = 0 ;
        /// -1: alloc as much as the buffer requires.
        size_t sib = size_t(-1) ;
    };
    so_typedef( memory_transfer_info ) ;
        
    struct memory_mapping_info
    {
        size_t offset = 0 ;
        /// -1: alloc as much as the buffer requires.
        size_t sib = size_t(-1) ;
    };
    so_typedef( memory_mapping_info ) ;

    struct memory_load_info
    {
        size_t offset = 0 ;
        size_t sib = size_t(-1) ;
    };
    so_typedef( memory_load_info ) ;
        
    struct render_config_info
    {
        render_config_info( so_gpu::config_ptr_t ptr, 
            size_t variable_set_index_ = 0, size_t num_primitives_ = size_t(-1) ) :
            config_ptr(ptr), variable_set_index(variable_set_index_), num_elements(num_primitives_)
        {}

        so_gpu::config_ptr_t config_ptr = nullptr ;
        size_t variable_set_index = 0 ;
            
        /// num vertices if vertex buffer only is used.
        /// num indices if index buffer is used.
        size_t num_elements = size_t(-1) ;
    };
    so_typedef( render_config_info ) ;
    
}

#endif

