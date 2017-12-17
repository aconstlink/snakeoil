//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "rect_render_2d.h"
#include "rect_render_2d_plug_factory.h"

#include <snakeoil/gpx/system/render_system.h>

using namespace so_gfx ;

//************************************************************************************
rect_render_2d::rect_render_2d( so_gpx::render_system_ptr_t rsptr )
{
    _gpxr = rsptr ;

    _sd_ptr = so_gfx::memory::alloc( so_gfx::rect_render_2d_shared_data_t(),
        "[rect_render_2d::rect_render_2d]" ) ;

    _fac_ptr = so_gfx::rect_render_2d_plug_factory_t::create(
        so_gfx::rect_render_2d_plug_factory_t( _sd_ptr ), "[rect_render_2d::rect_render_2d_plug_factory]" ) ;
}

//************************************************************************************
rect_render_2d::rect_render_2d( this_rref_t rhv )
{
    so_move_member_ptr( _fac_ptr, rhv ) ;
    so_move_member_ptr( _sd_ptr, rhv ) ;
    so_move_member_ptr( _gpxr, rhv ) ;

    _t_rnd = rhv._t_rnd ;
    rhv._t_rnd = so_gpx::technique_id_t( -1 ) ;

    _group_infos = std::move( rhv._group_infos ) ;
}

//************************************************************************************
rect_render_2d::~rect_render_2d( void_t )
{
    if( _t_rnd != so_gpx::technique_id_t( -1 ) )
    {
        _gpxr->unregister_technique( _t_rnd ) ;
    }

    so_gfx::memory::dealloc( _sd_ptr ) ;

    for( auto * ptr : _group_infos )
    {
        so_gfx::memory::dealloc( ptr ) ;
    }
}

//************************************************************************************
void_t rect_render_2d::init( void_t )
{
    if( _t_rnd == so_gpx::technique_id_t( -1 ) )
        _t_rnd = _gpxr->register_technique( _fac_ptr ) ;
}

//************************************************************************************
void_t rect_render_2d::set_view_projection( so_math::mat4f_cref_t view, so_math::mat4f_cref_t proj )
{
    _proj = proj ;
    _view = view ;
}

//************************************************************************************
rect_render_2d::this_ptr_t rect_render_2d::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//************************************************************************************
void_t rect_render_2d::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//************************************************************************************
so_gfx::result rect_render_2d::prepare_for_rendering( void_t )
{
    // 1. clear out the shared data
    {
        _sd_ptr->rect_infos.clear() ;
        _sd_ptr->per_group_infos.clear() ;
        _render_groups.clear() ;
    }

    for( auto * gptr : _group_infos )
    {
        // 1. copy group infos
        {
            so_gfx::rect_render_2d_shared_data_t::per_group_info_t pgi ;

            pgi.group_id = gptr->group_id ;
            pgi.num_rects = gptr->rect_infos.size() ;
            //pgi.proj = 
            //pgi.view = 

            _sd_ptr->per_group_infos.push_back( pgi ) ;
        }

        // 2. copy line infos
        {
            for( auto const l : gptr->rect_infos )
            {
                so_gfx::rect_render_2d_shared_data_t::rect_info_t ri ;

                ri.color = l.color ;
                ri.pos = l.pos;
                ri.scale = l.scale ;

                _sd_ptr->rect_infos.push_back( ri ) ;
            }
        }

        _render_groups.push_back( gptr->group_id ) ;
        gptr->rect_infos.clear() ;
    }

    // copy global data
    {
        _sd_ptr->proj = _proj ;
        _sd_ptr->view = _view ;
    }

    return so_gfx::ok ;
}

//************************************************************************************
so_gfx::result rect_render_2d::draw_rect( size_t const group_id, so_math::vec2f_cref_t pos, so_math::vec2f_cref_t scale,
    float_t const rot, so_math::vec4f_cref_t color )
{
    group_info_ptr_t gi_ptr = nullptr ;

    // 1. find and/or insert group
    {
        so_thread::lock_guard_t lk( _mtx_group ) ;
        auto iter = std::find_if( _group_infos.begin(), _group_infos.end(), [&] ( group_info_ptr_t gi )
        {
            return gi->group_id == group_id ;
        } ) ;

        if( iter == _group_infos.end() )
        {
            this_t::group_info_t gi ;

            gi.group_id = group_id ;
            //gi.proj = 
            //gi.view = 

            gi_ptr = so_gfx::memory::alloc( this_t::group_info_t( std::move( gi ) ),
                "[rect_render_2d::draw_lines] : group_infos" ) ;

            auto const lower_iter = std::lower_bound( _group_infos.begin(), _group_infos.end(), group_id,
                [&] ( this_t::group_info_ptr_t li, size_t const val )
            {
                return li->group_id < val;
            } ) ;

            _group_infos.insert( lower_iter, gi_ptr ) ;
        }
        else
            gi_ptr = *iter ;
    }

    // 2. generate user lines and insert into group's buffer
    {
        this_t::rect_info_t ri ;
        ri.color = color ;
        ri.pos = pos ;
        ri.rot = rot ;
        ri.scale = scale ;

        so_thread::lock_guard_t lk( gi_ptr->mtx ) ;
        gi_ptr->rect_infos.push_back( ri ) ;
    }
    return so_gfx::ok ;
}

//************************************************************************************
bool_t rect_render_2d::need_to_render( size_t const gid ) const
{
    for( auto id : _render_groups )
    {
        if( id < gid ) continue ;
        if( id > gid ) break ;
        return true ;
    }

    return false ;
}

//************************************************************************************
so_gfx::result rect_render_2d::render( size_t const gid )
{
    {
        bool_t const b = this_t::need_to_render( gid ) ;
        if( so_core::is_not( b ) )
            return so_gfx::ok ;
    }

    {
        so_gpx::schedule_instance_t si ;
        si.render_id = gid ;
        _gpxr->schedule( _t_rnd, 0, si ) ;
    }

    return so_gfx::ok ;
}

//************************************************************************************
so_gfx::result rect_render_2d::release( void_t )
{
    _gpxr->schedule_for_release( _t_rnd ) ;
    return so_gfx::ok ;
}