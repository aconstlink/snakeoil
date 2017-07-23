//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "api_object_helper.h"
#include "../object.h"

using namespace so_gpu ;

//*****************************************************************************************************
void_t api_object_helper::set_driver_object( so_gpu::object_ptr_t eng_obj, so_gpu::api_object_ptr_t drv_obj )
{
    eng_obj->set_driver_object( drv_obj ) ;
}

//*****************************************************************************************************
so_gpu::api_object_ptr_t api_object_helper::get_driver_object( so_gpu::object_ptr_t ptr )
{
    return ptr != nullptr ? ptr->get_driver_object() : nullptr ;
}

//*****************************************************************************************************
so_gpu::api_object_cptr_t api_object_helper::get_driver_object( so_gpu::object_cptr_t ptr )
{
    return ptr != nullptr ? ptr->get_driver_object() : nullptr ;
}

//*****************************************************************************************************
void_t api_object_helper::assert_no_driver_object( so_gpu::object_cptr_t obj_ptr )
{
    so_assert( this_t::get_driver_object( obj_ptr ) == nullptr ) ;
}

//*****************************************************************************************************
bool_t api_object_helper::has_driver_object( so_gpu::object_cptr_t obj_ptr )
{
    return this_t::get_driver_object( obj_ptr ) != nullptr ;
}

//*****************************************************************************************************
bool_t api_object_helper::has_no_driver_object( so_gpu::object_cptr_t obj_ptr )
{
    return this_t::get_driver_object( obj_ptr ) == nullptr ;
}

