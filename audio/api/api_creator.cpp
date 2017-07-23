//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "api_creator.h"

#include "null/null_api.h"

#if defined(XAUDIO2_ENABLE)
#include "xaudio2/xaudio2_api.h"
#endif

#if defined( WASAPI_ENABLE )
#include "wasapi/wasapi_api.h"
#endif

using namespace so_audio ;

//*************************************************************************************
bool_t api_creator::create_defaults( apis_out_t apis_out ) 
{

#if defined( WASAPI_ENABLE )
    {
        so_audio::so_win32::wasapi_api_t apio ;
        so_audio::so_win32::wasapi_api_ptr_t api_ptr = so_audio::so_win32::wasapi_api_t::create(
            std::move(apio), "[bool_t api_creator::create_defaults] : wasapi_api" ) ;

        apis_out.push_back( api_ptr ) ;
    }
#endif

#if defined( XAUDIO2_ENABLE )

    {
        so_audio::so_win32::xaudio2_api_t apio ;
        so_audio::so_win32::xaudio2_api_ptr_t api_ptr = so_audio::so_win32::xaudio2_api_t::create(
            std::move( apio ), "[bool_t api_creator::create_defaults] : xaudio2_api" ) ;

        apis_out.push_back( api_ptr ) ;
    }

#endif

    {
        so_audio::null_api_t apio ;
        so_audio::null_api_ptr_t api_ptr = so_audio::null_api_t::create(
            std::move( apio ), "[bool_t api_creator::create_defaults] : null_api" ) ;

        apis_out.push_back( api_ptr ) ;
    }

    return true ;
}