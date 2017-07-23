//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_BUFFER_LOOPBACK_BUFFER_H_
#define _SNAKEOIL_AUDIO_BUFFER_LOOPBACK_BUFFER_H_

#include "ibuffer.h"

namespace so_audio
{
    class SNAKEOIL_AUDIO_API loopback_buffer : public ibuffer
    {
        so_this_typedefs( loopback_buffer ) ;

    private:

        ipcm_buffer_ptr_t _pcm_ptr = nullptr ;

    public:

        loopback_buffer( void_t ) ;
        loopback_buffer( this_cref_t ) = delete ;
        loopback_buffer( this_rref_t ) ;
        virtual ~loopback_buffer( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t set_pcm_buffer( ipcm_buffer_ptr_t ) ;
        ipcm_buffer_ptr_t get_pcm_buffer( void_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( loopback_buffer ) ;
}

#endif