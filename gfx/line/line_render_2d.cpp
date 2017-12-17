//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "line_render_2d.h"
#include "line_render_2d_plug_factory.h"

using namespace so_gfx ;

//************************************************************************************
line_render_2d::line_render_2d( so_gpx::render_system_ptr_t rsptr )
{
    _gpxr = rsptr ;

    _sd_ptr = so_gfx::memory::alloc( so_gfx::line_render_2d_shared_data_t(), 
        "[line_render_2d::line_render_2d]" ) ;

    _fac_ptr = so_gfx::line_render_2d_plug_factory_t::create(
        so_gfx::line_render_2d_plug_factory_t( _sd_ptr ), "[line_render_2d::line_render_2d]" ) ;
}

//************************************************************************************
line_render_2d::line_render_2d( this_rref_t rhv )
{
    so_move_member_ptr( _fac_ptr, rhv ) ;
    so_move_member_ptr( _sd_ptr, rhv ) ;
    so_move_member_ptr( _gpxr, rhv ) ;

    _t_rnd = rhv._t_rnd ;
    rhv._t_rnd = so_gpx::technique_id_t( -1 ) ;

    _group_infos = std::move( rhv._group_infos ) ;
}

//************************************************************************************
line_render_2d::~line_render_2d( void_t )
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
void_t line_render_2d::init( void_t )
{
    if( _t_rnd == so_gpx::technique_id_t( -1 ) )
        _t_rnd = _gpxr->register_technique( _fac_ptr ) ;
}

//************************************************************************************
line_render_2d::this_ptr_t line_render_2d::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//************************************************************************************
void_t line_render_2d::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//************************************************************************************
void_t line_render_2d::prepare_for_rendering( void_t )
{
    // 1. clear out the shared data
    {
        _sd_ptr->line_infos.clear() ;
        _sd_ptr->per_group_infos.clear() ;
        _render_groups.clear() ;
    }

    for( auto * gptr : _group_infos )
    {
        // 1. copy group infos
        {
            so_gfx::line_render_2d_shared_data_t::per_group_info_t pgi ;

            pgi.group_id = gptr->group_id ;
            pgi.num_lines = gptr->line_infos.size() ;
            //pgi.proj = 
            //pgi.view = 
            pgi.width = gptr->line_width ;

            _sd_ptr->per_group_infos.push_back( pgi ) ;
        }

        // 2. copy line infos
        {
            for( auto const l : gptr->line_infos )
            {
                so_gfx::line_render_2d_shared_data_t::line_info_t li ;

                li.color = l.color ;
                li.pos1 = l.pos1;
                li.pos0 = l.pos0 ;

                _render_groups.push_back( gptr->group_id ) ;
                _sd_ptr->line_infos.push_back( li ) ;
            }
        }

        gptr->line_infos.clear() ;
    }
}

//************************************************************************************
bool_t line_render_2d::need_to_render( size_t const gid ) const
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
void_t line_render_2d::draw_lines( size_t const group_id, size_t const num_lines, draw_line_funk_t  f )
{
    group_info_ptr_t gi_ptr = this_t::find_or_create_group_info( group_id ) ;

    // 2. generate user lines and insert into group's buffer
    {
        so_thread::lock_guard_t lk( gi_ptr->mtx ) ;
        for( size_t i = 0; i < num_lines; ++i )
        {
            gi_ptr->line_infos.push_back( f( i ) ) ;
        }
    }
}

//************************************************************************************
void_t line_render_2d::set_line_width( size_t const group_id, float_t const width )
{
    group_info_ptr_t gi_ptr = this_t::find_or_create_group_info( group_id ) ;

    gi_ptr->line_width = width ;
}

//************************************************************************************
line_render_2d::group_info_ptr_t line_render_2d::find_or_create_group_info( size_t const group_id )
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
                "[line_render_2d::find_or_create_group_info] : group_infos" ) ;

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

    return gi_ptr ;
}

//************************************************************************************
void_t line_render_2d::render( size_t const gid )
{
    {
        bool_t const b = this_t::need_to_render( gid ) ;
        if( so_core::is_not( b ) )
            return ;
    }

    so_gpx::schedule_instance_t si ;
    si.render_id = gid ;
    _gpxr->schedule( _t_rnd, 0, si ) ;
}

//************************************************************************************
void_t line_render_2d::release( void_t )
{
    _gpxr->schedule_for_release( _t_rnd ) ;
}