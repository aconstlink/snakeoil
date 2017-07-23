//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_API_NULL_NULL_API_H_
#define _SNAKEOIL_AUDIO_API_NULL_NULL_API_H_

#include "../iapi.h"

namespace so_audio
{
    class SNAKEOIL_AUDIO_API null_api : public iapi
    {
        so_this_typedefs( null_api ) ;

    public:

        null_api( void_t ) ;
        null_api( this_cref_t ) = delete ;
        null_api( this_rref_t ) ;
        virtual ~null_api( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public: // iapi interface

        virtual bool_t initialize( void_t ) ;
        virtual void_t release( void_t ) ;
        virtual api_type get_type( void_t ) const ;
        virtual void_t destroy( void_t ) ;

    };
    so_typedef( null_api ) ;
}

#endif