//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "glyph_atlas.h"

using namespace so_font ;


//*******************************************************************
glyph_atlas::glyph_atlas( void_t )
{

}

//*******************************************************************
glyph_atlas::glyph_atlas( this_rref_t rhv )
{
    _glyph_infos = std::move( rhv._glyph_infos ) ;
    _atlas = std::move( rhv._atlas ) ;
}

//*******************************************************************
glyph_atlas::~glyph_atlas( void_t )
{
    for( auto * iptr : _atlas )
        so_font::memory::dealloc( iptr ) ;
}

//*******************************************************************
glyph_atlas::this_ptr_t glyph_atlas::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_font::memory::alloc( std::move( rhv ), p ) ;
}

//*******************************************************************
void_t glyph_atlas::destroy( this_ptr_t ptr )
{
    so_font::memory::dealloc( ptr ) ;
}

//*******************************************************************
bool_t glyph_atlas::add_glyph( glyph_info_cref_t gi )
{
    utf32_t const cp = gi.code_point ;
    size_t const fid = gi.face_id ;

    // check if already in glyph infos
    {
        auto const iter = std::find_if( _glyph_infos.begin(), _glyph_infos.end(), 
            [&]( glyph_info_cref_t rhv )
        {
            return rhv.code_point == cp && rhv.face_id == fid ;
        } ) ;

        if( iter != _glyph_infos.end() )
            return false ;
    }

    _glyph_infos.push_back( gi ) ;

    return true ;
}

//*******************************************************************
bool_t glyph_atlas::add_image( this_t::image_rref_t img )
{
    _atlas.push_back( so_font::memory::alloc( std::move(img), "[glyph_atlas::add_image]" ) ) ;
    return true ;
}

//*******************************************************************
size_t glyph_atlas::get_num_glyphs( void_t ) const
{
    return _glyph_infos.size() ;
}

//*******************************************************************
bool_t glyph_atlas::get_glyph_info( size_t const i, glyph_info_out_t gi ) const
{
    if( i >= _glyph_infos.size() ) return false ;
    gi = _glyph_infos[ i ] ;
    return true ;
}
//*******************************************************************
size_t glyph_atlas::get_num_images( void_t ) const
{
    return _atlas.size() ;
}

//*******************************************************************
bool_t glyph_atlas::find_glyph( size_t const fid, utf32_t const cp, 
    size_t & i_out, this_t::glyph_info_ref_t gi_out ) const
{
    auto const iter = std::find_if( _glyph_infos.begin(), _glyph_infos.end(), 
        [&]( glyph_info_cref_t gi_in )
    {
        return gi_in.face_id == fid && gi_in.code_point == cp ;
    } ) ;

    if( iter == _glyph_infos.end() )
        return false ;

    i_out = iter - _glyph_infos.begin() ;
    gi_out = *iter ;

    return true ;
}

//*******************************************************************
glyph_atlas::image_cptr_t glyph_atlas::get_image( size_t const id ) const
{
    if( id >= _atlas.size() ) return nullptr ;
    return _atlas[ id ] ;
}

//*******************************************************************
size_t glyph_atlas::get_width( void_t ) const
{
    return _atlas.size() > 0 ? _atlas[ 0 ]->width() : 0 ;
}

//*******************************************************************
size_t glyph_atlas::get_height( void_t ) const
{
    return _atlas.size() > 0 ? _atlas[ 0 ]->height() : 0 ;
}