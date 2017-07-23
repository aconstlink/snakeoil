//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "app.h"
#include "../system/iappx_context.h"

using namespace so_appx ;

//***********************************************************************
app::app( void_t ) 
{

}

//***********************************************************************
app::app( this_rref_t rhv ) 
{
    _ctx_ptr = rhv._ctx_ptr ;
    rhv._ctx_ptr = nullptr ;
}

//***********************************************************************
app::~app( void_t ) 
{

}

//***********************************************************************
so_appx::iappx_context_ptr_t app::ctx( void_t ) 
{
    return _ctx_ptr ;
}

//***********************************************************************
so_appx::result app::on_initialize( 
    so_appx::iappx_context_ptr_t ptr, so_appx::init_data_cref_t idata )
{
    _ctx_ptr = ptr ;
    return this->on_initialize(idata) ;
}

//***********************************************************************    
so_device::idevice_system_ptr_t app::dev( void_t ) 
{
    return _ctx_ptr->dev() ;
}

//***********************************************************************    
so_imex::isystem_ptr_t app::imexsys( void_t ) 
{
    return _ctx_ptr->imexsys() ;
}

//***********************************************************************    
so_gpx::render_system_ptr_t app::rsys( void_t )
{
    return _ctx_ptr->render_system() ;
}