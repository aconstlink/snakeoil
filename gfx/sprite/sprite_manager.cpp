//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "sprite_manager.h"

#include <snakeoil/log/global.h>

using namespace so_gfx ;

//************************************************************************************
sprite_manager::sprite_manager( so_gfx::image_render_2d_ptr_t img_rnd )
{
    _img_rnd = img_rnd ;
}

//************************************************************************************
sprite_manager::sprite_manager( this_rref_t rhv )
{
    so_move_member_ptr( _img_rnd, rhv ) ;

    _ssds = std::move( rhv._ssds ) ;
}

//************************************************************************************
sprite_manager::~sprite_manager( void_t )
{

}

//************************************************************************************
sprite_manager::this_ptr_t sprite_manager::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//************************************************************************************
void_t sprite_manager::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//************************************************************************************
so_gfx::result sprite_manager::add_sequence( so_imex::image_ptr_t img_ptr, so_std::string_cref_t key, sprite_infos_in_t infos )
{
    if( infos.size() == 0 ) return so_gfx::invalid_argument ;

    this_t::sprite_sequence_data_t ssd ;

    // 1. create sequence data
    {
        for( size_t i = 0; i < infos.size(); ++i )
        {
            this_t::sprite_data_t sd ;

            uint_t const yt = infos[ i ].yt() ;
            uint_t const yb = infos[ i ].yb() ;
            uint_t const xl = infos[ i ].xl() ;
            uint_t const xr = infos[ i ].xr() ;

            // in this case, y is measured from top
            // we need it measured from bottom though
            if( yb > yt )
            {
                uint_t const ih = uint_t( img_ptr->height ) ;

                uint_t const x = xl ;
                uint_t const y = ih - yb ;
                uint_t const w = xr - xl ;
                uint_t const h = yb - yt ;

                sd.img_id = _img_rnd->add_image( img_ptr, so_gpu::viewport_2d( x, y, w, h ) ) ;
                sd.scale = so_math::vec2f_t( float_t( w ), float_t( h ) ) ;
                sd.pivot = 
                    so_math::vec2f_t(infos[ i ].pivot) / so_math::vec2f_t(float_t(w),float_t(h)) * 
                    so_math::vec2f_t( 0.5f ) + so_math::vec2f_t( 0.5f )  ;
            }
            else
            {
                uint_t const x = xl ;
                uint_t const y = yb ;
                uint_t const w = xr - xl ;
                uint_t const h = yt - yb ;

                sd.img_id = _img_rnd->add_image( img_ptr, so_gpu::viewport_2d( x, y, w, h ) ) ;
                sd.scale = so_math::vec2f_t( float_t( w ), float_t( h ) ) ;
                sd.pivot = 
                    so_math::vec2f_t(infos[ i ].pivot) / so_math::vec2f_t(float_t(w),float_t(h))  * 
                    so_math::vec2f_t( 0.5f ) + so_math::vec2f_t( 0.5f ) ;
            }
            
            ssd.sds.push_back( sd )  ;
        }
    }

    // 2. add sequence info
    {
        so_thread::lock_guard_t lk( _mtx_ssds ) ;

        auto const iter = _ssds.find( key ) ;
        if( iter != _ssds.end() )
        {
            so_log::global_t::error("[sprite_manager::add_sequence] : sequence already exists.") ;
            return so_gfx::invalid_key ;
        }

        _ssds[ key ] = std::move( ssd ) ;
    }
   
    return so_gfx::ok ;
}

//************************************************************************************
so_gfx::result sprite_manager::set_sequence( so_imex::image_ptr_t img_ptr, 
    so_std::string_cref_t key, sprite_infos_in_t infos ) 
{
    {
        so_thread::lock_guard_t lk( _mtx_ssds ) ;

        auto const iter = _ssds.find( key ) ;
        if( iter != _ssds.end() )
        {
            for( size_t i=0; i<iter->second.sds.size(); ++i )
            {
                _img_rnd->remove_image( iter->second.sds[ i ].img_id ) ;
            }
            _ssds.erase( iter ) ;
        }
    }
    
    return this_t::add_sequence( img_ptr, key, infos ) ;
}

//************************************************************************************
bool_t sprite_manager::get_sequence( so_std::string_cref_t key, sprite_datas_out_t vo ) const
{
    auto const iter = _ssds.find( key ) ;
    if( iter == _ssds.end() )
        return false ;

    vo = iter->second.sds ;

    return true ;
}