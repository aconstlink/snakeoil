//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "null_window_handle.h"

using namespace so_app ;
using namespace so_app::so_null ;

//***********************************************************************
null_window_handle::null_window_handle( void_t )
{
}

//***********************************************************************
null_window_handle::null_window_handle( this_rref_t ) 
{
}

//***********************************************************************
null_window_handle::~null_window_handle( void_t )
{

}

//***********************************************************************
null_window_handle::this_ref_t null_window_handle::operator = ( this_rref_t )
{
    return *this ;
}