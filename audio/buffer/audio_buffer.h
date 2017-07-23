//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_BUFFER_AUDIO_BUFFER_H_
#define _SNAKEOIL_AUDIO_BUFFER_AUDIO_BUFFER_H_

#include "../api.h"
#include "../protos.h"
#include "../typedefs.h"

#include "../object.h"

namespace so_audio
{
    class SNAKEOIL_AUDIO_API audio_buffer : public so_audio::object
    {
        typedef so_audio::object base_t ;

        so_this_typedefs( audio_buffer ) ;

    private:

        so_audio::ipcm_buffer_ptr_t _pcm_ptr = nullptr ;

    public:

        audio_buffer( void_t ) ;
        audio_buffer( so_audio::api_object_ptr_t, so_audio::ipcm_buffer_ptr_t ) ;
        audio_buffer( this_cref_t ) = delete ;
        audio_buffer( this_rref_t ) ;
        virtual ~audio_buffer( void_t ) ;

    public:

        this_ref_t operator = ( this_rref_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

        so_audio::ipcm_buffer_ptr_t get_pcm_buffer( void_t ) ;
        so_audio::ipcm_buffer_cptr_t get_pcm_buffer( void_t ) const ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( audio_buffer ) ;
}

#endif