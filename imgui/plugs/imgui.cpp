//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "imgui.h"
#include "imgui_plug_factory.h"

#include <imgui/imgui.h>

using namespace so_imgui ;

//*************************************************************************************
imgui::imgui( so_std::string_cref_t name, so_gpx::render_system_ptr_t rs )
{
    _sd = so_imgui::memory::alloc( this_t::shared_data_t(),
        "[imgui::imgui] : shared_data" ) ;
    _sd->name = name ;
    _rs = rs ;

    _tid = _rs->register_technique( imgui_plug_factory_t::create(
        imgui_plug_factory_t( _sd ),
        "[imgui::imgui] : imgui_plug_factory_t" ) ) ;
}

//*************************************************************************************
imgui::imgui( this_rref_t rhv )
{
    so_move_member_ptr( _sd, rhv ) ;
    so_move_member_ptr( _rs, rhv ) ;
    _tid = std::move( rhv._tid ) ;
}

//*************************************************************************************
imgui::~imgui( void_t )
{
    if( _tid != size_t(-1) && so_core::is_not_nullptr(_rs) )
        _rs->schedule_for_release( _tid ) ;

    so_imgui::memory::dealloc( _sd ) ;
}

//*************************************************************************************
imgui::this_ptr_t imgui::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imgui::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t imgui::destroy( this_ptr_t ptr )
{
    so_imgui::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t imgui::init( void_t )
{
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    uchar_ptr_t pixels;
    int_t width, height;
    int_t bpp ;

    io.Fonts->GetTexDataAsRGBA32( &pixels, &width, &height, &bpp );
    io.Fonts->TexID = ( void * ) ( intptr_t ) 1;

    _sd->width = size_t(width) ;
    _sd->height = size_t(height) ;
    _sd->pixels = pixels ;

    _rs->schedule_for_init( _tid ) ;
}

//*************************************************************************************
void_t imgui::deinit( void_t )
{
    _rs->schedule_for_release( _tid ) ;
}

//*************************************************************************************
void_t imgui::schedule( so_gpx::window_id_t wid, ImDrawData * dd )
{
    _sd->dd = dd ;

    _rs->schedule( _tid, wid ) ;
}

//*************************************************************************************