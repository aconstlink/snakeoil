//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "log.h"
#include <snakeoil/log/global.h>

using namespace so_gpu ;
using namespace so_gpu::so_gl ;

//*************************************************************************************
bool_t so_gl::log::error( const char * msg )
{
    GLenum __so__err = so_gli::gl::glGetError() ; 
    if( __so__err == GL_NO_ERROR ) return false ;

    const std::string __glstring = std::to_string(__so__err) ; 
    so_log::global::error( std::string(msg) + " " + __glstring ) ;
    return true ;            
}

//*************************************************************************************
bool_t so_gl::log::error( std::string const & msg )
{
    return this_t::error( msg.c_str() ) ;
}

//*************************************************************************************
bool_t so_gl::log::error( bool_t condition, GLenum gle, const char * msg )
{
    if( condition )
    {
        const std::string __glstring = std::to_string(gle) ; 
        so_log::global::error( std::string(msg) + " " + __glstring ) ; 
    }
    return condition ;
}

//*************************************************************************************
bool_t so_gl::log::error( bool_t condition, GLenum gle, std::string const & msg )
{
    return this_t::error( condition, gle, msg.c_str() ) ;
}

