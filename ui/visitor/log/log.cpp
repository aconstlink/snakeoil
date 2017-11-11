
#include "log.h"

#include "../../node/layout/area_2d.h"
#include "../../node/layout/row_layout.h"
#include "../../node/layout/column_layout.h"
#include "../../node/layout/layer_2d.h"

#include <snakeoil/log/global.h>

#include <sstream>
#include <iomanip>

using namespace so_ui ;
using namespace so_ui::so_visitor ;

log::this_ptr_t log::create( void_t )
{
    return so_ui::memory::alloc( this_t(), 
        "[so_visitor::log::create]" ) ;
}

void_t log::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

void_t log::destroy( void_t )
{
    this_t::destroy( this ) ;
}

so_ui::result log::visit( so_ui::so_node::node_ptr_t )
{
    so_log::global::status( "-> node" ) ;

    return so_ui::ok ;
}

so_ui::result log::post_visit( so_ui::so_node::node_ptr_t )
{
    so_log::global::status( "<- node" ) ;
    return so_ui::ok ;
}

so_ui::result log::visit( so_ui::so_node::group_ptr_t )
{
    so_log::global::status( "-> group" ) ;
    return so_ui::ok ;
}

so_ui::result log::post_visit( so_ui::so_node::group_ptr_t )
{
    so_log::global::status( "<- group" ) ;
    return so_ui::ok ;
}

so_ui::result log::visit( so_ui::so_node::decorator_ptr_t )
{
    so_log::global::status( "-> decorator" ) ;
    return so_ui::ok ;
}

so_ui::result log::post_visit( so_ui::so_node::decorator_ptr_t )
{
    so_log::global::status( "<- decorator" ) ;
    return so_ui::ok ;
}

so_ui::result log::visit( so_ui::so_node::leaf_ptr_t )
{
    so_log::global::status( "-> leaf" ) ;
    return so_ui::ok ;
}

so_ui::result log::post_visit( so_ui::so_node::leaf_ptr_t )
{
    so_log::global::status( "<- leaf" ) ;
    return so_ui::ok ;
}

so_ui::result log::visit( so_ui::so_node::transform_2d_ptr_t )
{
    so_log::global::status( "-> transform_2d" ) ;
    return so_ui::ok ;
}

so_ui::result log::post_visit( so_ui::so_node::transform_2d_ptr_t )
{
    so_log::global::status( "<- transform_2d" ) ;
    return so_ui::ok ;
}

so_ui::result log::visit( so_ui::so_node::render_state_ptr_t )
{
    so_log::global::status( "-> render_state" ) ;
    return so_ui::ok ;
}

so_ui::result log::post_visit( so_ui::so_node::render_state_ptr_t )
{
    so_log::global::status( "<- render_state" ) ;
    return so_ui::ok ;
}

so_ui::result log::visit( so_ui::so_node::renderable_ptr_t )
{
    so_log::global::status( "-> renderable" ) ;
    return so_ui::ok ;
}

so_ui::result log::visit( so_ui::so_node::row_layout_ptr_t nptr )
{
    size_t const num_children = nptr->get_num_children() ;

    so_std::string_t str_print = "weights: " ;

    for( size_t i = 0; i < num_children; ++i )
    {
        float_t const weight = nptr->get_weight( i ) ;

        std::ostringstream sstr ;
        sstr << std::setprecision( 4 ) << weight ;

        str_print += sstr.str() + "; "  ;
    }

    so_log::global::status( str_print ) ;

    return so_ui::ok ;
}

so_ui::result log::visit( so_ui::so_node::column_layout_ptr_t )
{

    return so_ui::ok ;
}

so_ui::result log::visit( so_ui::so_node::area_2d_ptr_t nptr )
{
    so_math::vec4f_t const area = nptr->get_area() ;
    
    so_std::string_t str_print = "area: " ;

    {
        std::ostringstream sstr ;
        sstr << std::setprecision( 2 ) << area.x() ;
        str_print += sstr.str() + "; " ;
    }

    {
        std::ostringstream sstr ;
        sstr << std::setprecision( 2 ) << area.y() ;
        str_print += sstr.str() + "; " ;
    }

    {
        std::ostringstream sstr ;
        sstr << std::setprecision( 2 ) << area.z() ;
        str_print += sstr.str() + "; " ;
    }

    {
        std::ostringstream sstr ;
        sstr << std::setprecision( 2 ) << area.w() ;
        str_print += sstr.str() + "; " ;
    }

    so_log::global::status( str_print ) ;

    return so_ui::ok ;
}

so_ui::result log::visit( so_ui::so_node::layer_2d_ptr_t lptr )
{
    so_log::global::status( "layer_2d: " + std::to_string( lptr->get_layer() )  ) ;

    return so_ui::ok ;
}

