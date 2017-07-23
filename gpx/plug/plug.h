//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPX_PLUG_PLUG_H_
#define _SNAKEOIL_GPX_PLUG_PLUG_H_

#include "iplug.h"
#include "../service/iglobal_service.h"
#include "../service/ilocal_service.h"

namespace so_gpx
{
    template< typename A >
    class plug : public iplug
    {
        so_typedefs( A, api ) ;
        so_this_typedefs( plug<A> ) ;

    private:

        api_ptr_t _api_ptr = nullptr ;

        so_gpx::iglobal_service_ptr_t _gservice_ptr = nullptr ;
        so_gpx::ilocal_service_ptr_t _lservice_ptr = nullptr ;

    public:

        plug( api_ptr_t aptr ) : _api_ptr( aptr )
        {}

        plug( this_cref_t ) = delete ;
        plug( this_rref_t rhv )
        {
            so_move_member_ptr( _api_ptr, rhv ) ;
            so_move_member_ptr( _gservice_ptr, rhv ) ;
            so_move_member_ptr( _lservice_ptr, rhv ) ;
        }

        virtual ~plug( void_t ){}

    protected:

        api_ptr_t api( void_t ) { return _api_ptr ; }
        so_gpx::iglobal_service_ptr_t global_service( void_t ) { return _gservice_ptr ; }
        so_gpx::ilocal_service_ptr_t local_service( void_t ) { return _gservice_ptr ; }

    public:

        virtual so_gpx::plug_result on_create( create_info_cref_t cdata )
        {
            _gservice_ptr = cdata.gs_ptr ;
            _lservice_ptr = cdata.ls_ptr ;
            return so_gpx::plug_result::ok ;
        }

        virtual so_gpx::plug_result on_load( void_t ) {
            return so_gpx::plug_result::ok ;
        }

        virtual so_gpx::plug_result on_unload( void_t ) {
            return so_gpx::plug_result::ok ;
        }

    public:

        virtual so_gpx::plug_result on_initialize( init_info_cref_t ) {
            return so_gpx::plug_result::ok ;
        }
        
        virtual so_gpx::plug_result on_release( void_t ) {
            return so_gpx::plug_result::ok ;
        }
        
        virtual so_gpx::plug_result on_transfer( void_t ) {
            return so_gpx::plug_result::ok ;
        }
        
        virtual so_gpx::plug_result on_execute( execute_info_cref_t ) {
            return so_gpx::plug_result::ok ;
        }

    public:

        virtual so_gpx::plug_result on_update( void_t ) {
            return so_gpx::plug_result::ok ;
        }

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( iplug ) ;
}

#endif