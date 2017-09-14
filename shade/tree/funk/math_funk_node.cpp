//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "math_funk_node.h"

#include "generic_funk_node.h"
#include "../leaf/arg_node.h"

#include <snakeoil/log/global.h>

using namespace so_shade ;
using namespace so_shade::so_tree ;

//*************************************************************************************
so_shade::so_tree::funk_node_ptr_t math_funk::create_cos( void_t ) 
{
    auto * funk_ptr = so_shade::so_tree::generic_funk_node_t::create(
        so_shade::so_tree::generic_funk_node_t("cos"),
        "[so_shade::so_tree::math_funk::create_cos] : generic_funk_node_t" ) ;

    {
        auto const res = funk_ptr->create_arg( "angle" ) ;
        so_log::global::error( so_shade::no_success(res), 
            "[so_shade::so_tree::math_funk::create_cos] : arg" ) ;
    }
    
    return funk_ptr ;
}

//*************************************************************************************
so_shade::so_tree::funk_node_ptr_t math_funk::create_smoothstep( void_t ) 
{
    auto * funk_ptr = so_shade::so_tree::generic_funk_node_t::create(
        so_shade::so_tree::generic_funk_node_t( "smoothstep" ),
        "[so_shade::so_tree::math_funk::create_smoothstep] : generic_funk_node_t" ) ;

    {
        auto const res = funk_ptr->create_arg( "a" ) ;
        so_log::global::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_smoothstep] : arg 1" ) ;
    }

    {
        auto const res = funk_ptr->create_arg( "b" ) ;
        so_log::global::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_smoothstep] : arg 2" ) ;
    }

    {
        auto const res = funk_ptr->create_arg( "x" ) ;
        so_log::global::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_smoothstep] : arg 3" ) ;
    }

    return funk_ptr ;
}

//*************************************************************************************
so_shade::so_tree::funk_node_ptr_t math_funk::create_step( void_t ) 
{
    auto * funk_ptr = so_shade::so_tree::generic_funk_node_t::create(
        so_shade::so_tree::generic_funk_node_t( "step" ),
        "[so_shade::so_tree::math_funk::create_step] : generic_funk_node_t" ) ;

    {
        auto const res = funk_ptr->create_arg( "a" ) ;
        so_log::global::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_step] : arg 1" ) ;
    }

    {
        auto const res = funk_ptr->create_arg( "x" ) ;
        so_log::global::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_step] : arg 2" ) ;
    }

    return funk_ptr ;
}

//*************************************************************************************
so_shade::so_tree::funk_node_ptr_t math_funk::create_pulse( void_t ) 
{
    auto * funk_ptr = so_shade::so_tree::generic_funk_node_t::create(
        so_shade::so_tree::generic_funk_node_t( "pulse" ),
        "[so_shade::so_tree::math_funk::create_pulse] : generic_funk_node_t" ) ;

    {
        auto const res = funk_ptr->create_arg( "a" ) ;
        so_log::global::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_pulse] : arg 1" ) ;
    }

    {
        auto const res = funk_ptr->create_arg( "b" ) ;
        so_log::global::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_pulse] : arg 2" ) ;
    }

    {
        auto const res = funk_ptr->create_arg( "x" ) ;
        so_log::global::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_pulse] : arg 3" ) ;
    }

    return funk_ptr ;
}

//*************************************************************************************
so_shade::so_tree::funk_node_ptr_t math_funk::create_dot( void_t ) 
{
    auto * funk_ptr = so_shade::so_tree::generic_funk_node_t::create(
        so_shade::so_tree::generic_funk_node_t( "dot" ),
        "[so_shade::so_tree::math_funk::create_dot] : generic_funk_node_t" ) ;

    {
        auto const res = funk_ptr->create_arg( "a" ) ;
        so_log::global::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_dot] : arg 1" ) ;
    }

    {
        auto const res = funk_ptr->create_arg( "b" ) ;
        so_log::global::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_dot] : arg 2" ) ;
    }

    return funk_ptr ;
}

//*************************************************************************************
so_shade::so_tree::funk_node_ptr_t math_funk::create_lerp( void_t ) 
{
    auto * funk_ptr = so_shade::so_tree::generic_funk_node_t::create(
        so_shade::so_tree::generic_funk_node_t( "lerp" ),
        "[so_shade::so_tree::math_funk::create_lerp] : generic_funk_node_t" ) ;

    {
        auto const res = funk_ptr->create_arg( "a" ) ;
        so_log::global::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_lerp] : arg 1" ) ;
    }

    {
        auto const res = funk_ptr->create_arg( "b" ) ;
        so_log::global::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_lerp] : arg 2" ) ;
    }

    {
        auto const res = funk_ptr->create_arg( "x" ) ;
        so_log::global::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_lerp] : arg 3" ) ;
    }

    return funk_ptr ;
}
