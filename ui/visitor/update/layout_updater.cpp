//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "layout_updater.h"

#include "../../node/leaf.h"
#include "../../node/group.h"

#include "../../component/layout/row_layout.h"
#include "../../component/layout/column_layout.h"
#include "../../component/transform/rect_transform.h"
#include "../../component/transform/size_requirement.h"

#include <snakeoil/math/vector/vector2.hpp>

#include <algorithm>

using namespace so_ui ;
using namespace so_ui::so_visitor ;

//****************************************************************************
layout_updater::layout_updater( void_t )
{
    _astack.push( so_math::vec4f_t( 0.0f, 0.0f, 1.0f, 1.0f ) ) ;
    _rstack.push( so_math::vec4f_t( 0.0f, 0.0f, 0.0f, 0.0f ) ) ;
}

//****************************************************************************
layout_updater::layout_updater( float_t const x, float_t const y, float_t const w, float_t const h )
{
    _astack.push( so_math::vec4f_t( x, y, w, h ) ) ;
    _rstack.push( so_math::vec4f_t( x, y, 0.0f, 0.0f ) ) ;
}

//****************************************************************************
layout_updater::layout_updater( so_math::vec4f_cref_t dims )
{
    _astack.push( dims ) ;
    _rstack.push( so_math::vec4f_t( dims.x(), dims.y(), 0.0f, 0.0f ) ) ;
}

//****************************************************************************
layout_updater::layout_updater( this_cref_t )
{

}

//****************************************************************************
layout_updater::layout_updater( this_rref_t rhv )
{
    _astack = std::move( rhv._astack ) ;
    _rstack = std::move( rhv._rstack ) ;
}

//****************************************************************************
layout_updater::~layout_updater( void_t )
{

}

//****************************************************************************
layout_updater::this_ptr_t layout_updater::create( so_memory::purpose_cref_t p )
{
    return so_ui::memory::alloc( this_t(), p ) ;
}

//****************************************************************************
void_t layout_updater::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//****************************************************************************
so_ui::result layout_updater::visit( so_ui::so_node::group_ptr_t gptr )
{
    {
        so_ui::so_component::rect_transform_ptr_t comp = nullptr ;
        if( so_core::is_not( gptr->find_component( comp ) ) )
        {
            comp = so_ui::so_component::rect_transform_t::create(
                so_ui::so_component::rect_transform_t( _astack.top() ), "[layout_updater::visit] : rect_transform" ) ;

            gptr->add_component( comp ) ;
        }

        _astack.push( comp->get_dims() ) ;
    }

    {
        so_ui::so_component::row_layout_ptr_t lptr ;
        if( gptr->find_component( lptr ) )
        {
            so_math::vec4f_t const dims = _astack.top() ;

            float_t available_height = dims.w() ;
            size_t const num_children = gptr->get_num_children() ;
            size_t rest_children = num_children ;
            
            // 1. give proper size for size_requirement components
            {
                for( size_t i = 0; i < num_children; ++i )
                {
                    so_ui::so_node::node_ptr_t nptr = gptr->get_child( i ) ;

                    so_ui::so_component::rect_transform_ptr_t rt_comp = nullptr ;
                    if( so_core::is_not( nptr->find_component( rt_comp ) ) )
                    {
                        rt_comp = so_ui::so_component::rect_transform_t::create( "rect_transform" ) ;
                        nptr->add_component( rt_comp ) ;
                    }

                    so_ui::so_component::size_requirement_ptr_t comp = nullptr ;
                    if( nptr->find_component( comp ) )
                    {
                        auto const sd = comp->get_size_data() ;
                        if( sd.st_height == so_ui::so_component::size_requirement_t::size_type::pixel )
                        {
                            available_height = std::max( available_height- sd.height, 0.0f ) ;
                            rt_comp->set_dims( so_math::vec4f_t( 0.0f, 0.0f, dims.z(), sd.height ) ) ;
                        }
                        --rest_children ;
                    }
                }
            }
            
            // 2. set rest sizes and positions
            {
                float_t const part_scale = 1.0f / float_t( rest_children ) ;

                float_t y_offset = dims.w() ;

                for( size_t i = 0; i < num_children; ++i )
                {
                    so_ui::so_node::node_ptr_t nptr = gptr->get_child( i ) ;

                    bool_t const has_sr = nptr->has_component<so_ui::so_component::size_requirement_ptr_t>() ;

                    so_ui::so_component::rect_transform_ptr_t rt_comp = nullptr ;
                    if( so_core::is_not( nptr->find_component( rt_comp ) ) )
                    {
                        rt_comp = so_ui::so_component::rect_transform_t::create( "rect_transform" ) ;
                        nptr->add_component( rt_comp ) ;
                    }

                    float_t height ;
                    so_ui::so_component::size_requirement_ptr_t sr_comp ;
                    if( nptr->find_component( sr_comp ) )
                        height = rt_comp->get_dims().w() ;
                    else
                        height = available_height * part_scale ;

                    y_offset -= height ;
                    rt_comp->set_dims( so_math::vec4f_t( 0.0f, y_offset, dims.z(), height ) ) ;
                }
            }
        }
    }

    {
        so_ui::so_component::column_layout_ptr_t lptr ;
        if( gptr->find_component( lptr ) )
        {
            so_math::vec4f_t const dims = _astack.top() ;

            float_t available_width = dims.z() ;
            size_t const num_children = gptr->get_num_children() ;
            size_t rest_children = num_children ;

            // 1. give proper size for size_requirement components
            {
                for( size_t i = 0; i < num_children; ++i )
                {
                    so_ui::so_node::node_ptr_t nptr = gptr->get_child( i ) ;

                    so_ui::so_component::rect_transform_ptr_t rt_comp = nullptr ;
                    if( so_core::is_not( nptr->find_component( rt_comp ) ) )
                    {
                        rt_comp = so_ui::so_component::rect_transform_t::create( "rect_transform" ) ;
                        nptr->add_component( rt_comp ) ;
                    }

                    so_ui::so_component::size_requirement_ptr_t comp = nullptr ;
                    if( nptr->find_component( comp ) )
                    {
                        auto const sd = comp->get_size_data() ;
                        if( sd.st_width == so_ui::so_component::size_requirement_t::size_type::pixel )
                        {
                            available_width = std::max( available_width - sd.width, 0.0f ) ;
                            rt_comp->set_dims( so_math::vec4f_t( 0.0f, 0.0f, sd.width, dims.w() ) ) ;
                        }
                        --rest_children ;
                    }
                }
            }

            // 2. set rest sizes and positions
            {
                float_t const part_scale = 1.0f / float_t( rest_children ) ;

                float_t x_offset = 0.0f ;

                for( size_t i = 0; i < num_children; ++i )
                {
                    so_ui::so_node::node_ptr_t nptr = gptr->get_child( i ) ;

                    bool_t const has_sr = nptr->has_component<so_ui::so_component::size_requirement_ptr_t>() ;

                    so_ui::so_component::rect_transform_ptr_t rt_comp = nullptr ;
                    if( so_core::is_not( nptr->find_component( rt_comp ) ) )
                    {
                        rt_comp = so_ui::so_component::rect_transform_t::create( "rect_transform" ) ;
                        nptr->add_component( rt_comp ) ;
                    }

                    float_t width ;
                    so_ui::so_component::size_requirement_ptr_t sr_comp ;
                    if( nptr->find_component( sr_comp ) )
                        width = rt_comp->get_dims().z() ;
                    else
                        width = available_width * part_scale ;

                    rt_comp->set_dims( so_math::vec4f_t( x_offset, 0.0f, width, dims.w() ) ) ;
                    x_offset += width ;
                }
            }
        }
    }

    return so_ui::ok ;
}

//****************************************************************************
so_ui::result layout_updater::post_visit( so_ui::so_node::group_ptr_t gptr )
{
    /*
    so_math::vec4f_t odims ;

    // 1. collect all rect transforms from the child nodes
    // and compute own rect.
    {
        size_t const num_children = gptr->get_num_children() ;
        if( num_children > 0 )
        {
            so_ui::so_component::rect_transform_ptr_t comp ;
            if( gptr->get_child(0)->find_component( comp ) )
            {
                odims = comp->get_dims() ;
                odims.zw( odims.xy() + odims.zw() ) ;
            }
        }

        for( size_t i = 1; i < num_children; ++i )
        {
            auto * nptr = gptr->get_child( i ) ;
            so_ui::so_component::rect_transform_ptr_t comp ;
            if( nptr->find_component( comp ) )
            {
                so_math::vec4f_t dims = comp->get_dims() ;

                so_math::vec2f_t xy = so_math::vec2f_t(
                    std::min( dims.x(), odims.x() ), std::min( dims.y(), odims.y() ) ) ;


                so_math::vec2f_t zw = so_math::vec2f_t(
                    std::max( dims.x()+dims.z(), odims.z() ), std::max( dims.y()+dims.w(), odims.w() ) ) ;

                odims = so_math::vec4f_t( xy, zw ) ;
            }
        }
    }

    // 2. check own rect transform
    {
        so_ui::so_component::rect_transform_ptr_t comp = nullptr ;
        if( so_core::is_not( gptr->find_component( comp ) ) )
        {
            comp = so_ui::so_component::rect_transform_t::create( "rect_transform" ) ;
            gptr->add_component( comp ) ;
        }
        comp->set_dims( so_math::vec4f_t( odims.xy(), odims.zw() - odims.xy() ) ) ;
    }*/

    {
        _astack.pop() ;
    }

    return so_ui::ok ;
}

//****************************************************************************
so_ui::result layout_updater::visit( so_ui::so_node::leaf_ptr_t nptr )
{
    so_ui::so_component::rect_transform_ptr_t rt_comp = nullptr ;

    // rect transform
    {
        if( so_core::is_not( nptr->find_component( rt_comp ) ) )
        {
            auto const dims = so_math::vec4f_t( so_math::vec2f_t(), _astack.top().zw() ) ;

            rt_comp = so_ui::so_component::rect_transform_t::create(
                so_ui::so_component::rect_transform_t ( dims ), "[layout_updater::visit] : rect_transform" ) ;
            nptr->add_component( rt_comp ) ;
        }
    }

    // size requirement
    {
        so_ui::so_component::size_requirement_ptr_t comp = nullptr ;
        if( nptr->find_component( comp ) )
        {
            auto dims = rt_comp->get_dims() ;

            auto const sd = comp->get_size_data() ;
            if( sd.st_width == so_ui::so_component::size_requirement_t::size_type::pixel )
            {
                dims.z() = sd.width ;
            }

            if( sd.st_height == so_ui::so_component::size_requirement_t::size_type::pixel )
            {
                dims.w() = sd.height ;
            }

            rt_comp->set_dims( dims ) ;
        }
    }
   
    return so_ui::ok ;
}

#if 0

//****************************************************************************
so_ui::result layout_updater::visit( so_ui::so_node::leaf_ptr_t ptr )
{

    // 1. what we have
    so_math::vec4f_t dims_a = _astack.pop() ;
    so_math::vec4f_t dims_r = _rstack.pop() ;

    // 2. what we need
    auto const sr = ptr->get_size_requirement() ;

    // 3. where are the contraints?

    // 4. calc new dims here
    {
        switch( sr.st_width )
        {
        case size_requirement::size_type::percent:
            //dims.z() = sr.width * dims.z() ;
            break ;

        case size_requirement::size_type::pixel:
            //dims.z() = sr.width ;
            break ;
        default:
            break;
        }

        switch( sr.st_height )
        {
        case size_requirement::size_type::percent:
            //dims.w() = sr.height * dims.w() ;
            break ;

        case size_requirement::size_type::pixel:
            // dims.y() = dims.y() + dims.w() - sr.height ;
            //dims.w() = sr.height ;
            break ;
        default:
            break;
        }
    }

    // 5. reinsert new dims
    //_astack.push( dims ) ;
    return so_ui::ok ;
}

//****************************************************************************
so_ui::result layout_updater::visit( so_ui::so_node::row_layout_ptr_t nptr )
{
    float_t const max_weight = nptr->get_max_weight() ;
    size_t const num_children = nptr->get_num_children() ;

    float_t y_offset = 0.0f ;

    for( size_t i = 0; i < num_children; ++i )
    {
        float_t const weight = nptr->get_weight(i) ;
        float_t const scale = weight / max_weight ;

        auto * aptr = reinterpret_cast< so_ui::so_node::area_2d_ptr_t>( nptr->get_child( i ) ) ;
        
        so_math::vec4f_t const dims = _astack.top() ;

        float_t const some_height = dims.w()*scale ;
        y_offset += some_height ;
        aptr->set_area( so_math::vec4f_t( dims.x(), (dims.y()+dims.w()) - y_offset, dims.z(), some_height ) ) ;
    }

    return so_ui::ok ;
}

//****************************************************************************
so_ui::result layout_updater::post_visit( so_ui::so_node::row_layout_ptr_t nptr )
{
    size_t const num_children = nptr->get_num_children() ;

    for( size_t i = 1; i < num_children; ++i )
    {
        auto * aptr_l = reinterpret_cast< so_ui::so_node::area_2d_ptr_t >( nptr->get_child( i - 1 ) ) ;
        auto * aptr = reinterpret_cast< so_ui::so_node::area_2d_ptr_t >( nptr->get_child( i ) ) ;

        auto const dims_l = aptr_l->get_area() ;
        auto dims = aptr->get_area() ;

        dims.y() = dims_l.y() - dims_l.w() ;

        aptr->set_area( dims ) ;
    }

    return so_ui::ok ;
}

//****************************************************************************
so_ui::result layout_updater::visit( so_ui::so_node::column_layout_ptr_t nptr )
{
    float_t const max_weight = nptr->get_max_weight() ;
    size_t const num_children = nptr->get_num_children() ;

    float_t x_offset = 0.0f ;

    for( size_t i = 0; i < num_children; ++i )
    {
        float_t const weight = nptr->get_weight( i ) ;
        float_t const scale = weight / max_weight ;

        auto * aptr = reinterpret_cast< so_ui::so_node::area_2d_ptr_t >( nptr->get_child( i ) ) ;

        so_math::vec4f_t const dims = _astack.top() ;

        float_t const some_width = dims.z()*scale ;
        
        aptr->set_area( so_math::vec4f_t( dims.x()+x_offset, dims.y(), some_width, dims.w() ) ) ;
        x_offset += some_width ;
    }

    return so_ui::ok ;
}

//****************************************************************************
so_ui::result layout_updater::post_visit( so_ui::so_node::column_layout_ptr_t nptr )
{    
    size_t const num_children = nptr->get_num_children() ;
    
    for( size_t i = 1; i < num_children; ++i )
    {
        auto * aptr_l = reinterpret_cast< so_ui::so_node::area_2d_ptr_t >( nptr->get_child( i-1 ) ) ;
        auto * aptr = reinterpret_cast< so_ui::so_node::area_2d_ptr_t >( nptr->get_child( i ) ) ;

        auto const dims_l = aptr_l->get_area() ;

        auto dims = aptr->get_area() ;

        dims.x() = dims_l.x() + dims_l.z()  ;
        //dims.y() = dims_l.y() - dims_l.w() ;

        aptr->set_area( dims ) ;
    }

    return so_ui::ok ;
}

//****************************************************************************
so_ui::result layout_updater::visit( so_ui::so_node::area_2d_ptr_t aptr )
{
    _astack.push( aptr->get_area() ) ;

    return so_ui::ok ;
}

//****************************************************************************
so_ui::result layout_updater::post_visit( so_ui::so_node::area_2d_ptr_t aptr )
{
    // may have been changed via 
    // a size requirement
    auto const dims = _astack.pop() ;

    aptr->set_area( dims ) ;

    // fix parent area
    {
        auto const dims_p = _astack.pop() ;

        //dims_p.x = 

        _astack.push( dims_p ) ;
    }

    return so_ui::ok ;
}

#endif

//****************************************************************************
void_t layout_updater::destroy( void_t )
{
    this_t::destroy( this ) ;
}