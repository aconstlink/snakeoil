//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "variable_node.h"
#include "../../slot/output/output_slot.hpp"
#include "../../slot/input/input_slot.hpp"

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>

#include <snakeoil/math/vector/vector2b.hpp>
#include <snakeoil/math/vector/vector3b.hpp>
#include <snakeoil/math/vector/vector4b.hpp>

#include <snakeoil/math/matrix/matrix2.hpp>
#include <snakeoil/math/matrix/matrix3.hpp>
#include <snakeoil/math/matrix/matrix4.hpp>

#include <snakeoil/variable/variable.hpp>
#include <snakeoil/log/log.h>

using namespace so_flow ;

//***********************************************************************
namespace so_this_file
{
    template< typename T >
    static bool_t try_create_input_slot( so_flow::variable_node_ptr_t vn_ptr, so_flow::key_in_t key_in,
        so_var::ivariable_ptr_t ivar_ptr, so_flow::result & res_out  )
    {
        if(so_core::is_not_nullptr( dynamic_cast<so_var::variable<T>*>(ivar_ptr) ))
        {
            so_typedefs( so_var::variable<T>, var ) ;
            var_ptr_t var_ptr = static_cast<var_ptr_t>(ivar_ptr) ;
            res_out = vn_ptr->create_input_slot( key_in, var_ptr->get_data_ptr() ) ;
            return true ;
        }
        return false ;
    }

    /// try all existing so_math vector types
    template< typename T >
    static bool_t try_create_vec_input_slot( so_flow::variable_node_ptr_t vn_ptr, so_flow::key_in_t key_in,
        so_var::ivariable_ptr_t ivar_ptr, so_flow::result & res_out )
    {
        if(so_core::is_not_nullptr( dynamic_cast<so_var::variable<so_math::vector2<T>>*>(ivar_ptr) ))
        {
            so_typedefs( so_var::variable<so_math::vector2<T>>, var ) ;
            var_ptr_t var_ptr = static_cast<var_ptr_t>(ivar_ptr) ;
            res_out = vn_ptr->create_input_slot( key_in, var_ptr->get_data_ptr() ) ;
            return true ;
        }
        else if(so_core::is_not_nullptr( dynamic_cast<so_var::variable<so_math::vector3<T>>*>(ivar_ptr) ))
        {
            so_typedefs( so_var::variable<so_math::vector3<T>>, var ) ;
            var_ptr_t var_ptr = static_cast<var_ptr_t>(ivar_ptr) ;
            res_out = vn_ptr->create_input_slot( key_in, var_ptr->get_data_ptr() ) ;
            return true ;
        }
        else if(so_core::is_not_nullptr( dynamic_cast<so_var::variable<so_math::vector4<T>>*>(ivar_ptr) ))
        {
            so_typedefs( so_var::variable<so_math::vector4<T>>, var ) ;
            var_ptr_t var_ptr = static_cast<var_ptr_t>(ivar_ptr) ;
            res_out = vn_ptr->create_input_slot( key_in, var_ptr->get_data_ptr() ) ;
            return true ;
        }
        return false ;
    }

    /// try all existing so_math matrix types
    template< typename T >
    static bool_t try_create_mat_input_slot( so_flow::variable_node_ptr_t vn_ptr, so_flow::key_in_t key_in,
        so_var::ivariable_ptr_t ivar_ptr, so_flow::result & res_out )
    {
        if( so_core::is_not_nullptr( dynamic_cast<so_var::variable<so_math::matrix2<T>>*>(ivar_ptr) ) )
        {
            so_typedefs( so_var::variable<so_math::matrix2<T>>, var ) ;
            var_ptr_t var_ptr = static_cast<var_ptr_t>(ivar_ptr) ;
            res_out = vn_ptr->create_input_slot( key_in, var_ptr->get_data_ptr() ) ;
            return true ;
        }
        else if( so_core::is_not_nullptr( dynamic_cast<so_var::variable<so_math::matrix3<T>>*>(ivar_ptr) ) )
        {
            so_typedefs( so_var::variable<so_math::matrix3<T>>, var ) ;
            var_ptr_t var_ptr = static_cast<var_ptr_t>(ivar_ptr) ;
            res_out = vn_ptr->create_input_slot( key_in, var_ptr->get_data_ptr() ) ;
            return true ;
        }
        else if( so_core::is_not_nullptr( dynamic_cast<so_var::variable<so_math::matrix4<T>>*>(ivar_ptr) ) )
        {
            so_typedefs( so_var::variable<so_math::matrix4<T>>, var ) ;
            var_ptr_t var_ptr = static_cast<var_ptr_t>(ivar_ptr) ;
            res_out = vn_ptr->create_input_slot( key_in, var_ptr->get_data_ptr() ) ;
            return true ;
        }
        return false ;
    }
}

//***********************************************************************
variable_node::variable_node( void_t ) 
{}

//***********************************************************************
variable_node::variable_node( this_rref_t )
{}

//***********************************************************************
variable_node::~variable_node( void_t )
{}

//***********************************************************************
variable_node::this_ptr_t variable_node::create( so_memory::purpose_cref_t p )
{
    return so_flow::memory::alloc( this_t(), p ) ;
}

//***********************************************************************
variable_node::this_ptr_t variable_node::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_flow::memory::alloc( std::move(rhv), p ) ;
}

//***********************************************************************
void_t variable_node::destroy( this_ptr_t ptr )
{
    so_flow::memory::dealloc( ptr ) ;
}

//***********************************************************************
so_flow::result variable_node::create_input_connections( so_var::variable_set_cref_t var_set ) 
{
    var_set.for_each( [&]( so_var::key_cref_t key_in, so_var::ivariable_ptr_t var_ptr )
    {
        auto res = this->create_input_connection( key_in, var_ptr ) ;
        so_log::log::warning( so_flow::no_success(res), 
            "[so_flow::variable_node::create_connections] : a varialbe couldn't be connected : " + 
            key_in ) ;
    } ) ;

    return so_flow::ok ;
}

//***********************************************************************
so_flow::result variable_node::create_input_connection( 
    so_flow::key_in_t key_in, so_var::ivariable_ptr_t ivar_ptr )
{
    if( so_core::is_nullptr(ivar_ptr) ) 
        return so_flow::invalid_pointer ;

    so_flow::result res = so_flow::failed ;

    if( so_this_file::try_create_input_slot<bool_t>( this, key_in, ivar_ptr, res ) ) return res ;
    else if( so_this_file::try_create_input_slot<float_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_input_slot<double_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_input_slot<char_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_input_slot<int_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_input_slot<uint_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_input_slot<short_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_input_slot<size_t>( this, key_in, ivar_ptr, res )  ) return res ;
    
    else if( so_this_file::try_create_input_slot<so_math::vec2b_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_input_slot<so_math::vec3b_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_input_slot<so_math::vec4b_t>( this, key_in, ivar_ptr, res )  ) return res ;


    else if( so_this_file::try_create_vec_input_slot<float_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_vec_input_slot<double_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_vec_input_slot<char_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_vec_input_slot<int_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_vec_input_slot<uint_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_vec_input_slot<short_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_vec_input_slot<size_t>( this, key_in, ivar_ptr, res )  ) return res ;
    
    else if( so_this_file::try_create_mat_input_slot<float_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_mat_input_slot<double_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_mat_input_slot<int_t>( this, key_in, ivar_ptr, res )  ) return res ;
    else if( so_this_file::try_create_mat_input_slot<uint_t>( this, key_in, ivar_ptr, res )  ) return res ;

    so_log::log::warning( "[so_flow::variable_node::create_connection] : type not supported" ) ;

    return so_flow::failed ;
}

//***********************************************************************
bool_t variable_node::on_update( void_t )
{
    return true ;
}

//***********************************************************************
void_t variable_node::destroy( void_t )
{
    this_t::destroy( this ) ;
}
