//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "image_render_2d.h"
#include "image_render_2d_plug_factory.h"

#include <snakeoil/gpx/system/render_system.h>

#include <snakeoil/imex/image/image.h>

#include <snakeoil/font/glyph_atlas/glyph_atlas.h>
#include <snakeoil/font/glyph_atlas/stb/stb_glyph_atlas_creator.h>

#include <snakeoil/log/global.h>

using namespace so_gfx ;

//************************************************************************************
image_render_2d::image_render_2d( so_gpx::render_system_ptr_t rsptr )
{
    _gpxr = rsptr ;

    _sd_ptr = so_gfx::memory::alloc( so_gfx::image_render_2d_shared_data_t(),
        "[image_render_2d::image_render_2d] : shared data" ) ;

    _fac_ptr = so_gfx::image_render_2d_plug_factory_t::create(
        so_gfx::image_render_2d_plug_factory_t( _sd_ptr ),
        "[image_render_2d::image_render_2d] : plug factory" ) ;
}

//************************************************************************************
image_render_2d::image_render_2d( this_rref_t rhv )
{
    so_move_member_ptr( _gpxr, rhv ) ;
    so_move_member_ptr( _sd_ptr, rhv ) ;
    so_move_member_ptr( _fac_ptr, rhv ) ;

    _gis = std::move( rhv._gis ) ;
    _images = std::move( rhv._images ) ;
}

//************************************************************************************
image_render_2d::~image_render_2d( void_t )
{
    if( _t_rnd != so_gpx::technique_id_t( -1 ) )
    {
        _gpxr->unregister_technique( _t_rnd ) ;
    }

    so_gfx::memory::dealloc( _sd_ptr ) ;

    for( auto * g : _gis )
    {
        for( auto * idti : g->ids_and_images )
        {
            so_gfx::memory::dealloc( idti ) ;
        }
        so_gfx::memory::dealloc( g ) ;
    }

    // do not destroy images. Those may come from anywhere else.
    /*
    for( auto & id : _images )
    {
        so_imex::image_t::destroy( image ) ;
    }
    */
}

//************************************************************************************
image_render_2d::this_ptr_t image_render_2d::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//************************************************************************************
void_t image_render_2d::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//************************************************************************************
void_t image_render_2d::init( so_math::mat4f_cref_t view, so_math::mat4f_cref_t proj )
{
    if( _t_rnd == so_gpx::technique_id_t(-1) )
        _t_rnd = _gpxr->register_technique( _fac_ptr ) ;

    _proj = proj ;
    _view = view ;

    _sd_ptr->proj = proj ;
    _sd_ptr->view = view ;
}

//************************************************************************************
image_render_2d::image_id_t image_render_2d::add_image( so_imex::image_ptr_t img_ptr )
{
    if( so_core::is_nullptr( img_ptr ) )
        return this_t::image_id_t() ;

    return this_t::add_image( img_ptr, so_gpu::viewport_2d_t( 0,0,img_ptr->width, img_ptr->height) ) ;
}

//************************************************************************************
image_render_2d::image_id_t image_render_2d::add_image( so_imex::image_ptr_t img_ptr, so_gpu::viewport_2d_cref_t vp )
{
    if( so_core::is_nullptr( img_ptr ) )
    {
        so_log::global::warning( "[image_render_2d::add_image] : image is nullptr" ) ;
        return this_t::image_id_t() ;
    }

    so_thread::lock_guard_t lk( _mtx_image ) ;
    {
        auto const iter = std::find_if( _images.begin(), _images.end(), [&]( image_data_cref_t id ) 
        {
            return id.img_ptr == img_ptr ;
        } ) ;

        if( iter != _images.end() )
        {
            size_t const iid = iter - _images.begin() ;
            iter->viewports.push_back( vp ) ;

            return this_t::image_id_t( iid, iter->viewports.size()-1 ) ;
        }
    }

    image_data_t id ;
    id.img_ptr = img_ptr ;
    id.viewports.push_back( vp ) ;
    _images.push_back( id ) ;

    return image_id_t( _images.size()-1, _images.back().viewports.size()-1 )  ;
}

//************************************************************************************
image_render_2d::group_info_ptr_t image_render_2d::insert_group( size_t const group )
{
    group_info_t li ;
    li.group_id = group ;
    li.proj = so_math::mat4f_t().identity() ;
    li.view = so_math::mat4f_t().identity() ;

    auto const lower_iter = std::lower_bound( _gis.begin(), _gis.end(), group,
        [&] ( this_t::group_info_ptr_t li, size_t const val )
    {
        return li->group_id < val;
    } ) ;
    
    return *( _gis.insert( lower_iter, so_gfx::memory::alloc( li ) ) ) ;
}

//************************************************************************************
void_t image_render_2d::set_view_projection( size_t const group, so_math::mat4f_cref_t view, so_math::mat4f_cref_t proj )
{
    so_thread::lock_guard_t lk( _mtx_gis ) ;

    this_t::group_info_ptr_t gptr = nullptr ;

    auto iter = std::find_if( _gis.begin(), _gis.end(), [&] ( this_t::group_info_ptr_t li )
    {
        return  li->group_id == group ;
    } ) ;

    gptr = *iter ;

    if( iter == _gis.end() )
    {
        gptr = this_t::insert_group( group ) ;
    }

    gptr->proj = proj ;
    gptr->view = view ;
}

//************************************************************************************
so_gfx::result image_render_2d::draw_image( size_t const group, image_id_cref_t image_id, bool_t const dirty,
    so_math::vec2f_cref_t pos, so_math::vec2f_cref_t pivot, so_math::vec2f_cref_t scale, float_t const rot,
    so_math::vec4f_cref_t color )
{
    if( so_core::is_not( image_id.is_valid() ) )
        return so_gfx::invalid_id ;

    // 1. transform text to glyphs
    // 2. add text to buffer

    group_info_ptr_t gptr = nullptr ;
    {
        so_thread::lock_guard_t lk( _mtx_gis ) ;
        auto const iter = std::find_if( _gis.begin(), _gis.end(), [&] ( this_t::group_info_ptr_t li )
        {
            return  li->group_id == group ;
        } ) ;
        

        if( iter == _gis.end() )
        {
            gptr = this_t::insert_group( group ) ;
            gptr->proj = _proj ;
            gptr->view = _view ;
        }
        else
        {
            gptr = *iter ;
        }
    }

    id_to_images_ptr_t id_to_images_ptr = nullptr ;
    {
        so_thread::lock_guard_t lk( gptr->mtx ) ;
        auto iter = std::find_if( gptr->ids_and_images.begin(), gptr->ids_and_images.end(), 
            [&] ( this_t::id_to_images_ptr_t item ) { return item->image_index == image_id._iid ; } ) ;

        if( iter == gptr->ids_and_images.end() )
        {
            this_t::id_to_images_t iti ;
            iti.image_index = image_id._iid ;
            id_to_images_ptr = so_gfx::memory::alloc( std::move( iti ), "[image_render_2d::draw_image] : id_to_images " ) ;

            gptr->ids_and_images.insert( gptr->ids_and_images.end(), id_to_images_ptr ) ;
        }
        else
            id_to_images_ptr = *iter ;
    }
    
    // image info
    {
        this_t::image_info_t ii ;
        ii.color = color ;
        ii.scale = scale ;
        ii.pos = pos ;
        ii.pivot = pivot ;
        ii.dirty = dirty ;
        ii.rot = rot ;
        ii.image_id = image_id ;

        so_thread::lock_guard_t lk( id_to_images_ptr->mtx ) ;
        id_to_images_ptr->image_infos.push_back( ii ) ;
    }
    
    return so_gfx::ok ;
}

//************************************************************************************
so_gfx::result image_render_2d::prepare_for_rendering( void_t )
{
    // 1. clear out the shared data
    {
        _sd_ptr->image_infos.resize( 0 ) ;
        _sd_ptr->per_group_infos.resize( 0 ) ;
        _render_groups.clear() ;
    }

    // 2. copy data to shared data object
    // 3. clear image infos
    {
        for( auto & p : _gis )
        {
            //so_thread::lock_guard_t lk( p->mtx ) ;

            for( auto * idai : p->ids_and_images )
            {
                // 1. per image, one group
                {
                    image_render_2d_shared_data_t::per_group_info pgi ;
                    pgi.group_id = p->group_id ;
                    pgi.num_images = idai->image_infos.size() ;

                    pgi.proj = p->proj ;
                    pgi.view = p->view ;

                    _render_groups.push_back( p->group_id ) ;
                    _sd_ptr->per_group_infos.push_back( pgi ) ;
                }

                for( auto const & ii : idai->image_infos )
                {
                    auto const * img_ptr = _images[ ii.image_id._iid ].img_ptr ;
                    auto const & vp = _images[ ii.image_id._iid ].viewports[ ii.image_id._vpid ] ;

                    float_t const iw = float_t( img_ptr->width ) ;
                    float_t const ih = float_t( img_ptr->height ) ;

                    so_math::vec4f_t const texcoords(
                        vp.get_x<float_t>() / iw,
                        vp.get_y<float_t>() / ih,
                        vp.get_width<float_t>() / iw,
                        vp.get_height<float_t>() / ih
                    ) ;

                    {
                        image_render_2d_shared_data_t::image_info_t sii ;
                        sii.dirty = ii.dirty ;
                        sii.img_ptr = img_ptr ;
                        sii.texcoords = texcoords ;
                        sii.pos = ii.pos ;
                        sii.rot = ii.rot ;
                        sii.pivot = ii.pivot ;
                        sii.scale = ii.scale * so_math::vec2f_t( iw, ih ) ;
                        sii.color = ii.color ;
                        _sd_ptr->image_infos.push_back( sii ) ;
                    }
                }
                idai->image_infos.clear() ;
            }
        }
    }
   
#if 0
    {
        so_math::vec2f_t const vp( _ci.vp.get_width<float_t>(), _ci.vp.get_height<float_t>() ) ;
        _sd_ptr->dim_scale = _glyph_atlas_size / vp ;
    }
#endif
    return so_gfx::ok ;
}

//************************************************************************************
bool_t image_render_2d::need_to_render( size_t const gid ) const
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
so_gfx::result image_render_2d::render( size_t const gid )
{
    {
        bool_t const b = this_t::need_to_render( gid ) ;
        if( so_core::is_not( b ) )
            return so_gfx::ok ;
    }

    so_gpx::schedule_instance_t si ;
    si.render_id = gid ;
    _gpxr->schedule( _t_rnd, 0, si ) ;

    return so_gfx::ok ;
}

//************************************************************************************
so_gfx::result image_render_2d::release( void_t )
{
    _gpxr->schedule_for_release( _t_rnd ) ;
    return so_gfx::ok ;
}