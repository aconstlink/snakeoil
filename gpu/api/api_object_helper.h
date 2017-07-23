//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_API_OBJECT_SETTER_H_
#define _SNAKEOIL_API_OBJECT_SETTER_H_

#include "../typedefs.h"
#include "../protos.h"

namespace so_gpu
{
    class api_object_helper
    {
        so_this_typedefs( api_object_helper ) ;
        
    public:

        static void_t set_driver_object( so_gpu::object_ptr_t, so_gpu::api_object_ptr_t ) ;
        static so_gpu::api_object_ptr_t get_driver_object( so_gpu::object_ptr_t ) ;

        static so_gpu::api_object_cptr_t get_driver_object( so_gpu::object_cptr_t ) ;

        static void_t assert_no_driver_object( so_gpu::object_cptr_t obj_ptr ) ;

        static bool_t has_driver_object( so_gpu::object_cptr_t obj_ptr ) ;
        static bool_t has_no_driver_object( so_gpu::object_cptr_t obj_ptr ) ;

    public: // reinterprete

        template< typename drv_type_t >
        static drv_type_t * get_cast_api_object( so_gpu::object_ptr_t ptr )
        {
            so_gpu::api_object_ptr_t drv_obj_ptr = this_t::get_driver_object( ptr ) ;
            so_assert( drv_obj_ptr != nullptr ) ;

            return reinterpret_cast< drv_type_t* >( drv_obj_ptr ) ;
        }

        template< typename drv_type_t >
        static drv_type_t const * get_cast_api_object( so_gpu::object_cptr_t ptr )
        {
            so_gpu::api_object_cptr_t drv_obj_ptr = this_t::get_driver_object( ptr ) ;
            so_assert( drv_obj_ptr != nullptr ) ;

            return reinterpret_cast< drv_type_t const * >( drv_obj_ptr ) ;
        }

    public: // dynamic

        template< typename drv_type_t >
        static bool_t is_of_api_object_type( so_gpu::object_ptr_t ptr )
        {
            return get_dyn_cast_api_object<drv_type_t>( ptr ) != nullptr ;
        }

        template< typename drv_type_t >
        static drv_type_t * get_dyn_cast_api_object( so_gpu::object_ptr_t ptr )
        {
            so_gpu::api_object_ptr_t drv_obj_ptr = this_t::get_driver_object( ptr ) ;
            so_assert( drv_obj_ptr != nullptr ) ;

            return dynamic_cast< drv_type_t* >( drv_obj_ptr ) ;
        }

        template< typename drv_type_t >
        static drv_type_t const * get_dyn_cast_api_object( so_gpu::object_cptr_t ptr )
        {
            so_gpu::api_object_cptr_t drv_obj_ptr = this_t::get_driver_object( ptr ) ;
            so_assert( drv_obj_ptr != nullptr ) ;

            return dynamic_cast< drv_type_t const * >( drv_obj_ptr ) ;
        }
    };
    so_typedef( api_object_helper ) ;
}

#endif