//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "texture_funk_node.h"

#include "generic_funk_node.h"
#include "../leaf/arg_node.h"

#include <snakeoil/log/log.h>

using namespace so_shade ;
using namespace so_shade::so_tree ;

//*************************************************************************************
so_shade::so_tree::funk_node_ptr_t texture_funk::create_texture_access( void_t ) 
{
    auto * funk_ptr = so_shade::so_tree::generic_funk_node_t::create(
        so_shade::so_tree::generic_funk_node_t( "pulse" ),
        "[so_shade::so_tree::math_funk::create_pulse] : generic_funk_node_t" ) ;

    {
        auto const res = funk_ptr->create_arg( "texture" ) ;
        so_log::log::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_smoothstep] : arg 1" ) ;
    }

    {
        auto const res = funk_ptr->create_arg( "texcoords" ) ;
        so_log::log::error( so_shade::no_success( res ),
            "[so_shade::so_tree::math_funk::create_smoothstep] : arg 2" ) ;
    }

    return funk_ptr ;
}
