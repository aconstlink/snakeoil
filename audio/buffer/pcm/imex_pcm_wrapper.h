//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_BUFFER_PCM_WRAPPER_H_
#define _SNAKEOIL_AUDIO_BUFFER_PCM_WRAPPER_H_

#include "ipcm_buffer.h"

#include <snakeoil/imex/audio/iaudio.h>

namespace so_audio
{
    class SNAKEOIL_AUDIO_API imex_pcm_wrapper : public ipcm_buffer
    {
        so_this_typedefs( imex_pcm_wrapper ) ;

    private:

        so_imex::iaudio_ptr_t _imex_audio_ptr = nullptr ;

    public:

        imex_pcm_wrapper( so_imex::iaudio_ptr_t ) ;
        imex_pcm_wrapper( this_cref_t ) = delete ;
        imex_pcm_wrapper( this_rref_t ) ;
        virtual ~imex_pcm_wrapper( void_t ) ;

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual double_t get_time_in_sec( void_t ) const ;
        virtual buffer_type get_buffer_type( void_t ) const ;

        virtual so_audio::range_t copy_from( ipcm_buffer_ptr_t, size_t const, size_t const ) ;
        virtual so_audio::range_t copy_from( byte_cptr_t, size_t const ) ;
        virtual so_audio::range_t copy_from( so_audio::buffer_type, byte_cptr_t, size_t const ) ;

        virtual void_t destroy( void_t ) ;

        virtual void_cptr_t get_buffer_ptr_at( size_t const ) const ;

        virtual void_cptr_t get_buffer_ptr( void_t ) const ;
        virtual size_t get_num_samples( void_t ) const ;
        virtual size_t get_num_samples_per_second( void_t ) const ;
        virtual size_t get_num_channels( void_t ) const ;

    };
    so_typedef( imex_pcm_wrapper ) ;
}

#endif