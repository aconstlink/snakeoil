#include "presentation.h"

#include "../post/post.h"

#include <snakeoil/gfx/render/render_2d.h>
#include <snakeoil/gfx/text/text_render_2d.h>
#include <snakeoil/gfx/plugs/framebuffer/predef_framebuffer.h>
#include <snakeoil/gpx/system/render_system.h>

#include <snakeoil/gpu/viewport/viewport_2d.h>

#include <snakeoil/math/utility/3d/orthographic_projection.hpp>

#include <snakeoil/log/global.h>

using namespace sox_presentation ;

//*********************************************************
bool_t presentation::page_info::on_load( void_t ) 
{
    if( !loaded ) loaded = pptr->on_load() ;
    return loaded ;
}

//*********************************************************
void_t presentation::page_info::on_unload( void_t ) 
{
    if( so_log::global_t::error( inited,
        "[presentation::page_info] : call on_release first" ) ) return ;

    if( loaded ) 
    {
        loaded = so_core::is_not( pptr->on_unload() ) ;
    }
}

//*********************************************************
bool_t presentation::page_info::on_init( void_t ) 
{
    if( !inited ) 
    {
        inited = pptr->on_init() ;
    }
    return inited ;
}

//*********************************************************
bool_t presentation::page_info::on_release( void_t ) 
{
    if( inited )
    {
        inited = so_core::is_not( pptr->on_release() ) ;
    }
    return inited ;
}

//*********************************************************
bool_t presentation::page_info::do_update( update_data_in_t ud ) 
{
    if( !on_load() ) return false ;
    if( !on_init() ) return false ;
    
    pptr->on_update( ud ) ;

    return true ;
}

//*********************************************************
bool_t presentation::page_info::do_render( render_data_in_t rd ) 
{
    if( !on_load() ) return false ;
    if( !on_init() ) return false ;

    pptr->on_render( rd ) ;

    return true ;
}

// transition info //
//*********************************************************
bool_t presentation::transition_info::on_load( void_t ) 
{
    if( !loaded ) loaded = pptr->on_load() ;
    return loaded ;
}

//*********************************************************
void_t presentation::transition_info::on_unload( void_t ) 
{
    if( loaded ) 
    {
        loaded = so_core::is_not( pptr->on_unload() ) ;
    }
}

//*********************************************************
bool_t presentation::transition_info::on_init( void_t ) 
{
    if( !inited ) 
    {
        inited = pptr->on_init() ;
    }
    return inited ;
}

//*********************************************************
bool_t presentation::transition_info::on_release( void_t ) 
{
    if( inited )
    {
        inited = so_core::is_not( pptr->on_release() ) ;
    }
    return inited ;
}

//*********************************************************
bool_t presentation::transition_info::do_update( update_data_in_t ud ) 
{
    if( !on_load() ) return false ;
    if( !on_init() ) return false ;
    
    pptr->on_update( ud ) ;

    return true ;
}

//*********************************************************
bool_t presentation::transition_info::do_render( 
    itransition::render_type const rt, render_data_in_t rd ) 
{
    if( !on_load() ) return false ;
    if( !on_init() ) return false ;

    pptr->on_render( rt, rd ) ;

    return true ;
}

//*********************************************************
presentation::presentation( so_gpx::render_system_ptr_t ptr ) noexcept 
{
    _rs = ptr ;

    _fb_c0 = so_gfx::predef_framebuffer_t::create( 
        so_gfx::predef_framebuffer_t( so_gfx::predef_framebuffer_type::color888_alpha8, _rs ),
        "[presentation::presentation] : predef framebuffer" ) ;

    _fb_c1 = so_gfx::predef_framebuffer_t::create(
        so_gfx::predef_framebuffer_t( so_gfx::predef_framebuffer_type::color888_alpha8, _rs ),
        "[presentation::presentation] : predef framebuffer" ) ;

    _fb_cx = so_gfx::predef_framebuffer_t::create(
        so_gfx::predef_framebuffer_t( so_gfx::predef_framebuffer_type::color888_alpha8, _rs ),
        "[presentation::presentation] : predef framebuffer" ) ;

    _fb_cm = so_gfx::predef_framebuffer_t::create(
        so_gfx::predef_framebuffer_t( so_gfx::predef_framebuffer_type::color888_alpha8, _rs ),
        "[presentation::presentation] : predef framebuffer" ) ;

    _fb_blt = so_gfx::predef_framebuffer_t::create(
        so_gfx::predef_framebuffer_t( so_gfx::predef_framebuffer_type::invalid, _rs ),
        "[presentation::presentation] : predef framebuffer blt" ) ;

    _post = sox_presentation::post_t::create( 
        sox_presentation::post_t(_rs ), 
        "[presentation::presentation] : sox_presentation::post_t" ) ;

    _rnd_2d = so_gfx::render_2d_t::create( so_gfx::render_2d_t( _rs ), 
        "[presentation::presentation] : so_gfx::render_2d_t" ) ;

    _txt_rnd = so_gfx::text_render_2d_t::create( so_gfx::text_render_2d_t( _rs ), 
        "[presentation::presentation] : so_gfx::render_2d_t" ) ;
}

//*********************************************************
presentation::presentation( this_rref_t rhv ) noexcept
{
    _pages = std::move( rhv._pages ) ;
    _transitions = std::move( rhv._transitions ) ;

    _cur_index = rhv._cur_index ;
    _tgt_index = rhv._tgt_index ;

    so_move_member_ptr( _fb_c0, rhv ) ;
    so_move_member_ptr( _fb_c1, rhv ) ;
    so_move_member_ptr( _fb_cx, rhv ) ;
    so_move_member_ptr( _fb_cm, rhv ) ;
    so_move_member_ptr( _fb_blt, rhv ) ;
    so_move_member_ptr( _post, rhv ) ;
    so_move_member_ptr( _rnd_2d, rhv ) ;
    so_move_member_ptr( _txt_rnd, rhv ) ;

    _vp = rhv._vp ;
}

//*********************************************************
presentation::~presentation( void_t ) noexcept
{
    for( auto & item : _pages )
    {
        item.pptr->destroy() ;
    }

    for( auto item : _transitions ) 
    {
        item.pptr->destroy() ;
    }

    so_gfx::predef_framebuffer_t::destroy( _fb_c0 ) ;
    so_gfx::predef_framebuffer_t::destroy( _fb_c1 ) ;
    so_gfx::predef_framebuffer_t::destroy( _fb_cx ) ;
    so_gfx::predef_framebuffer_t::destroy( _fb_cm ) ;
    so_gfx::predef_framebuffer_t::destroy( _fb_blt ) ;
    so_gfx::render_2d_t::destroy( _rnd_2d ) ;
    so_gfx::text_render_2d_t::destroy( _txt_rnd ) ;

    sox_presentation::post_t::destroy( _post ) ;
}

//*********************************************************
presentation::this_ptr_t presentation::create( this_rref_t rhv, so_memory::purpose_cref_t p ) noexcept
{
    return so_memory::global_t::alloc( std::move( rhv ), p ) ;
}

//*********************************************************
void_t presentation::destroy( this_ptr_t ptr ) noexcept
{
    so_memory::global_t::dealloc( ptr ) ;
}

//*********************************************************
void_t presentation::init( so_gpu::viewport_2d_cref_t vp, so_io::path_cref_t path ) noexcept 
{
    _vp = vp ;

    _fb_c0->init( "presentation.scene.0", _vp.get_width(), _vp.get_height() ) ;
    _fb_c0->schedule_for_init() ;
    _fb_c1->init( "presentation.scene.1", _vp.get_width(), _vp.get_height() ) ;
    _fb_c1->schedule_for_init() ;
    _fb_cx->init( "presentation.cross", _vp.get_width(), _vp.get_height() ) ;
    _fb_cx->schedule_for_init() ;
    _fb_cm->init( "presentation.mask", _vp.get_width(), _vp.get_height() ) ;
    _fb_cm->schedule_for_init() ;
    _fb_blt->init( "scene", _vp.get_width(), _vp.get_height(), 0 ) ;
    _post->init( "presentation.scene.0", "presentation.scene.1", 
       "presentation.cross", "presentation.mask", "" ) ;

    _rnd_2d->init() ;
    _txt_rnd->init_fonts( 50, { path } ) ;
}

//*********************************************************
void_t presentation::render( void_t ) noexcept
{
    {
        so_gfx::text_render_2d_t::canvas_info_t ci ;
        ci.vp = _vp ; // so_gpu::viewport_2d_t( 0, 0, 1920, 1080 ) ;

        _txt_rnd->set_canvas_info( ci ) ;
        //_txt_rnd->set_view_projection( so_math::mat4f_t().identity(),
            //so_math::mat4f_t().identity() ) ;
          //  so_math::so_3d::orthographic<float_t>::create( 1920.0f, 1080.0f, 0.1f, 1000.0f ) ) ;
    }

    render_data_t rd ;
    rd.rnd_2d = _rnd_2d ;
    rd.txt_rnd = _txt_rnd ;

    size_t layer_start = 0 ;
    size_t layer_end = 10 ;

    _rnd_2d->prepare_for_rendering() ;
    _txt_rnd->prepare_for_rendering() ;

    // 1. do current page
    {
        this_t::cur_page( [&] ( page_info_ref_t pi )
        {
            layer_start = 0 ;
            layer_end = 10 ;
            _fb_c0->set_clear_color( so_math::vec4f_t(1.0f, 1.0f, 1.0f, 1.0f) ) ;
            
            _fb_c0->schedule_for_begin() ;
            _fb_c0->schedule_for_clear() ;

            pi.do_render( rd ) ;
            
            for( size_t i=layer_start; i<=layer_end; ++i )
            {
                rd.rnd_2d->render( i ) ;
                rd.txt_rnd->render( i ) ;
            }
            _fb_c0->schedule_for_end() ;
        } ) ;
    }

    // 2. do transition
    if( this_t::in_transition() )
    {
        bool_t const res = this_t::cur_transition( [&] ( transition_info_ref_t ti )
        {
            {
                layer_start = layer_end + 1 ;
                layer_end = layer_start + 10 ;

                _fb_cx->set_clear_color( so_math::vec4f_t( 0.0f,0.0f,1.0f,1.0f ) ) ;
                _fb_cx->schedule_for_begin() ;
                _fb_cx->schedule_for_clear() ;
                
                ti.do_render( itransition::render_type::scene, rd ) ;
                
                for( size_t i = layer_start; i <= layer_end; ++i )
                {
                    rd.rnd_2d->render( i ) ;
                    rd.txt_rnd->render( i ) ;
                }
                _fb_cx->schedule_for_end() ;
                
            }
            #if 0
            {
                // how to say that the mask should be rendered?
                _fb_cm->schedule_for_begin() ;
                _fb_cm->schedule_for_clear() ;
                ti.do_render( itransition::render_type::mask, rd ) ;
            }
            #endif
        } ) ;
    }

    //#if 0
    // 3. do next page
    if( this_t::in_transition() )
    {
        layer_start = layer_end + 1 ;
        layer_end = layer_start + 10 ;


        this_t::tgt_page( [&] ( page_info_ref_t pi )
        {
            _fb_c1->set_clear_color( so_math::vec4f_t( 1.0f, 0.0f, 0.0f, 1.0f ) ) ;
            _fb_c1->schedule_for_begin() ;
            _fb_c1->schedule_for_clear() ;

            pi.do_render( rd ) ;
            //#if 0
            for( size_t i = layer_start; i <= layer_end; ++i )
            {
                rd.rnd_2d->render( i ) ;
                rd.txt_rnd->render( i ) ;
            }
            //#endif
            _fb_c1->schedule_for_end() ;
        } ) ;
    }
    //#endif 
    // 4. do post
    {
        _fb_blt->schedule_for_begin() ;
        _post->render( this_t::in_transition() ) ;
    }
}

//*********************************************************
void_t presentation::update( void_t ) noexcept
{
    auto const dt = std::chrono::duration_cast<std::chrono::microseconds>( 
        so_core::clock_t::now() - _utime )  ;

    _utime = so_core::clock_t::now() ;

    update_data_t ud ;
    ud.rnd_2d = _rnd_2d ;
    ud.txt_rnd = _txt_rnd ;

    // 0. check for abort transition
    if( _abort_transition )
    {
        change_to_target() ;
        _abort_transition = false ;
    }

    std::chrono::microseconds dur(0) ;

    // x. check transition
    if( this_t::in_transition() )
    {
        this_t::cur_transition( [&] ( transition_info_ref_t ti )
        {
            dur = std::chrono::duration_cast< std::chrono::microseconds >(
                ti.pptr->get_duration() ) ;
        } ) ;

        // is transition done?
        if( dur <= _tdur )
        {
            change_to_target() ;
            _tdur = std::chrono::microseconds( 0 ) ;
        }
    }

    // 1. do current page
    {
        ud.layer_start = 0 ;
        ud.layer_end = 10 ;

        this_t::cur_page( [&] ( page_info_ref_t pi )
        {
            pi.do_update( ud ) ;
        } ) ;
    }

    // 2. do transition
    {
        this_t::cur_transition( [&] ( transition_info_ref_t ti )
        {
            ti.on_load() ;
        } ) ;

        if( this_t::in_transition() )
        {
            ud.layer_start = ud.layer_end + 1 ;
            ud.layer_end = ud.layer_start + 10 ;

            this_t::cur_transition( [&] ( transition_info_ref_t ti )
            {
                ti.do_update( ud ) ;
            } ) ;
        }
    }

    // 3. do target page
    {
        this_t::tgt_page( [&] ( page_info_ref_t pi )
        {
            pi.on_load() ;

            if( this_t::in_transition() )
            {
                ud.layer_start = ud.layer_end + 1 ;
                ud.layer_end = ud.layer_start + 10 ;
                pi.do_update( ud ) ;
            }
                
        } ) ;
    }

    // 4. guess to load the next page
    {
        this_t::nxt_page( [&] ( page_info_ref_t pi )
        {
            pi.on_load() ;
        } ) ;
    }

    // 5. do unload
    {
        size_t i ;
        if( this_t::cur_index( i ) && i > 2 )
        {
            _pages[ i - 2 ].on_unload() ;
        }
    }

    // 6. update transition duration
    {
        _tdur += dt ;
    }
    
}

//*********************************************************
void_t presentation::release( void_t ) 
{
    _fb_c0->schedule_for_release() ;
    _fb_c1->schedule_for_release() ;
    _fb_cx->schedule_for_release() ;
    _fb_cm->schedule_for_release() ;
}

//*********************************************************
void_t presentation::change_to_target( void_t ) noexcept
{
    this_t::cur_page( [&] ( page_info_ref_t pi )
    {
        pi.on_release() ;
    } ) ;

    this_t::cur_transition( [&] ( transition_info_ref_t ti )
    {
        ti.on_release() ;
    } ) ;

    _cur_index = _tgt_index ;

    this_t::cur_page( [&] ( page_info_ref_t pi )
    {
        pi.on_init() ;
    } ) ;
}

//*********************************************************
void_t presentation::add_page( sox_presentation::ipage_utr_t pptr ) noexcept
{
    {
        auto const iter = std::find_if( _pages.begin(), _pages.end(), [&]( page_info_cref_t item )
        {
            return item.pptr == pptr ;
        } ) ;
        if( iter != _pages.end() )
            return ;
    }

    {
        this_t::page_info_t pi ;
        pi.loaded = false ;
        pi.pptr = pptr ;
        _pages.emplace_back( pi ) ;
    }

    if( _cur_index == size_t( -1 ) )
    {
        _cur_index = 0 ;
        _tgt_index = 0 ;
    }
}

//*********************************************************
void_t presentation::add_transition( sox_presentation::itransition_utr_t ptr ) noexcept 
{
    {
        auto const iter = std::find_if( _transitions.begin(), _transitions.end(), 
            [&]( transition_info_cref_t item )
        {
            return item.pptr == ptr ;
        } ) ;

        if( iter != _transitions.end() )
            return ;
    }

    {
        this_t::transition_info_t i ;
        i.loaded = false ;
        i.pptr = ptr ;
        _transitions.emplace_back( i ) ;
    }
}

//*********************************************************
bool_t presentation::next_page( void_t ) noexcept 
{
    if( this_t::in_transition() )
    {
        _abort_transition = true ;
    }
    else
    {
        this_t::nxt_index( _tgt_index ) ;
    }

    _tdur = std::chrono::microseconds( 0 ) ;

    return _tgt_index != size_t( -1 ) ;
}

//*********************************************************
bool_t presentation::prev_page( void_t ) noexcept 
{
    if( this_t::in_transition() )
    {
        _abort_transition = true ;
    }
    else 
    {
        this_t::prv_index( _tgt_index ) ;
    }
    
    _tdur = std::chrono::microseconds( 0 ) ;

    return _tgt_index != size_t( -1 ) ;
}

//*********************************************************
bool_t presentation::change_page( size_t const ) noexcept 
{
    return false ;
}

