//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_SPLINE_SPLINE_MANAGER_H_
#define _SNAKEOIL_MANAGER_SPLINE_SPLINE_MANAGER_H_

#include "../api.h"
#include "../result.h"
#include "../typedefs.h"
#include "../protos.h"

#include <snakeoil/thread/typedefs.h>

#include <snakeoil/resource/handle/handle.hpp>
#include <snakeoil/resource/manager/resource_manager.hpp>

namespace so_manager
{
    class SNAKEOIL_MANAGER_API spline_manager
    {
        so_this_typedefs( spline_manager ) ;

    private:

        struct linear_spline_store_item
        {
            // linear spline
        };
        so_typedef( linear_spline_store_item ) ;

        so_typedefs( so_resource::resource_manager<linear_spline_store_item>, linears_resource_manager ) ;
        linears_resource_manager_t _linears_res_mgr ;

    private: // more resource managers here

    public:

        so_typedefs( linears_resource_manager_t::handle_t, linears_handle ) ;
        so_typedefs( linears_resource_manager_t::handles_t, linears_handles ) ;

        struct linears_manage_params
        {
            // linear_spline
        };
        so_typedef( linears_manage_params ) ;

    private:

        so_thread::mutex_t _mtx_counter ;
        size_t _counter = 0 ;

    public:

        spline_manager( void_t ) ;
        spline_manager( this_rref_t ) ;
        spline_manager( this_cref_t ) = delete ;

        virtual ~spline_manager( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        bool_t acquire( so_manager::key_cref_t key_in,
            so_resource::purpose_cref_t p, linears_handle_out_t hnd_out ) ;

        bool_t release( linears_handle_rref_t hnd ) ;

    public: // insert/reserve/exchange

        /// does not acquire the resource. It only inserts it.
        so_manager::result insert( so_manager::key_cref_t key_in, linears_manage_params_cref_t mp ) ;
        
    };
}

#endif