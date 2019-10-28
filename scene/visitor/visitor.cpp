
#include "visitor.h"

using namespace so_scene ;
using namespace so_scene::so_visitor ;

so_scene::result visitor::visit( so_scene::so_node::node_ptr_t )
{
    return so_scene::result::ok ;
}

so_scene::result visitor::post_visit( so_scene::so_node::node_ptr_t )
{
    return so_scene::result::ok ;
}

so_scene::result visitor::visit( so_scene::so_node::group_ptr_t )
{
    return so_scene::result::ok ;
}

so_scene::result visitor::post_visit( so_scene::so_node::group_ptr_t )
{
    return so_scene::result::ok ;
}

so_scene::result visitor::visit( so_scene::so_node::decorator_ptr_t )
{
    return so_scene::result::ok ;
}

so_scene::result visitor::post_visit( so_scene::so_node::decorator_ptr_t )
{
    return so_scene::result::ok ;
}

so_scene::result visitor::visit( so_scene::so_node::leaf_ptr_t )
{
    return so_scene::result::ok ;
}

so_scene::result visitor::post_visit( so_scene::so_node::leaf_ptr_t )
{
    return so_scene::result::ok ;
}

so_scene::result visitor::visit( so_scene::so_node::transform_3d_ptr_t )
{
    return so_scene::result::ok ;
}

so_scene::result visitor::post_visit( so_scene::so_node::transform_3d_ptr_t )
{
    return so_scene::result::ok ;
}

so_scene::result visitor::visit( so_scene::so_node::render_state_ptr_t )
{
    return so_scene::result::ok ;
}

so_scene::result visitor::post_visit( so_scene::so_node::render_state_ptr_t )
{
    return so_scene::result::ok ;
}

so_scene::result visitor::visit( so_scene::so_node::renderable_ptr_t )
{
    return so_scene::result::ok ;
}

so_scene::result visitor::visit( so_scene::so_node::camera_ptr_t )
{
    return so_scene::result::ok ;
}