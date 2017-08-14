//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_MANAGER_GPU_MANAGER_H_
#define _SNAKEOIL_GPU_MANAGER_GPU_MANAGER_H_

#include "../api.h"
#include "../typedefs.h"
#include "../result.h"
#include "../protos.h"

#include <snakeoil/resource/handle/handle.hpp>
#include <snakeoil/resource/manager/resource_manager.hpp>

namespace so_gpu
{
    class SNAKEOIL_GPU_API gpu_manager
    {
        so_this_typedefs( gpu_manager ) ;

    public:

        so_typedefs( so_resource::resource_manager< so_gpu::framebuffer_2d >, fb2d_manager ) ;
        so_typedefs( so_resource::resource_manager< so_gpu::texture_2d >, tx2d_manager ) ;
        so_typedefs( so_resource::resource_manager< so_gpu::iimage_2d >, img2d_manager ) ;
        

        so_typedefs( so_resource::resource_manager< so_gpu::ivertex_buffer>, vb_manager ) ;
        so_typedefs( so_resource::resource_manager< so_gpu::iindex_buffer>, ib_manager ) ;

    private:

        fb2d_manager_ptr_t _fb2d_mgr = nullptr ;
        tx2d_manager_ptr_t _tx2d_mgr = nullptr ;
        img2d_manager_ptr_t _img2d_mgr = nullptr ;

        vb_manager_ptr_t _vb_mgr = nullptr ;
        ib_manager_ptr_t _ib_mgr = nullptr ;

    public:

        gpu_manager( void_t ) ;
        gpu_manager( this_cref_t ) = delete ;
        gpu_manager( this_rref_t ) ;
        ~gpu_manager( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:
        
        fb2d_manager_ptr_t get_fb2d_mgr( void_t ) ;
        tx2d_manager_ptr_t get_tx2d_mgr( void_t ) ;
        img2d_manager_ptr_t get_img2d_mgr( void_t ) ;

        vb_manager_ptr_t get_vb_mgr( void_t ) ;
        ib_manager_ptr_t get_ib_mgr( void_t ) ;
    };
    so_typedef( gpu_manager ) ;
}

#endif