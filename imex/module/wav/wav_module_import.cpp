//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "wav_module.h"

#include "../../audio/pcm_audio.hpp"
#include "../../manager/manager_registry.h"
#include "../../manager/audio_manager.h"

#include <snakeoil/io/io.h>
#include <snakeoil/io/system/system.h>

#include <snakeoil/memory/guards/malloc_guard.hpp>
#include <snakeoil/thread/task/tasks.h>

#include <snakeoil/log/log.h>

using namespace so_imex ;

namespace this_file
{
    /// informational chunk
    struct chunk
    {
        char_t id[ 5 ] = { 'x', 'x', 'x', 'x', '\0' } ;
        uint32_t size = 0 ;
    };
    so_typedef( chunk ) ;
    so_typedefs( so_std::vector<chunk_t>, chunks ) ;

    struct riff_chunk
    {
        uint32_t size = 0 ;
    };

    struct fmt_chunk
    {
        // 16, 18, 40
        uint32_t size ;

        // pcm : 1
        // ieee_float : 3
        // a-law : 6
        // u-law : 7
        // 0xfffe : extensible 
        uint16_t format ;

        uint16_t channels ;
        uint32_t samples_per_sec ;
        uint32_t avg_bytes_per_sec ;
        uint16_t block_align ;
        uint16_t bits_per_sample ;
        uint16_t ext_size ;
        uint16_t valid_bits_per_sample ;
        uint32_t channel_mask ;
    };

    struct list_chunk
    {
        uint32_t size = 0 ;
    };

    struct data_chunk
    {
        uint32_t size = 0 ;
    };

    struct wav_file
    {
        chunks_t chunks ;

        riff_chunk riff ;
        fmt_chunk fmt ;
        list_chunk list ;
        data_chunk data ;
    };
}

//**********************************************************************************************
so_thread::task_graph_t wav_module::import_audio( import_params_cref_t params_in ) 
{
    so_imex::sync_object::set_not_signal( params_in.sync_ptr, so_imex::not_ready ) ;

    so_thread::void_funk_task_ptr_t import_task_ptr =
        so_thread::void_funk_task_t::create( [=]( void_t )
    {
        so_memory::malloc_guard<char_t> data_buffer ;

        // 1. load the file into a buffer
        {
            so_io::load_handle_t lh = so_io::io::load( params_in.path_to_file ) ;
            auto res = lh.wait_for_operation( [&]( char_cptr_t din, size_t sib, so_io::result ires )
            {
                if( so_io::no_success( ires ) ) return ;
                data_buffer = so_memory::malloc_guard<char_t>( din, sib ) ;
            } ) ;

            if( so_log::log::error( so_io::no_success( res ),
                "[so_imex::wav_module::import_audio] : loading wav file : " +
                so_io::to_string( res ) ) )
            {
                return so_imex::sync_object::set_and_signal(
                    params_in.sync_ptr, so_imex::failed ) ;
            }
        }

        this_file::wav_file wf ;

        // only RIFF and WAVE supported
        {
            uint32_t const fmt = uint32_t( *uint32_ptr_t( &data_buffer[ 0 ] ) ) ;
            uint32_t const ft = uint32_t( *uint32_ptr_t( &data_buffer[ sizeof( uint32_t ) << 1 ] ) ) ;

            char_t fmt_buffer[ 5 ] = { 'x', 'x', 'x', 'x', '\0' } ;
            char_t ft_buffer[ 5 ] = { 'x', 'x', 'x', 'x', '\0' } ;

            for( size_t i = 0; i < 4; ++i ) {
                fmt_buffer[ i ] = ( fmt >> ( i << 3 ) ) & 255 ;
                ft_buffer[ i ] = ( ft >> ( i << 3 ) ) & 255 ;
            }

            if( fmt_buffer[0] != 'R' || fmt_buffer[ 1 ] != 'I' || fmt_buffer[ 2 ] != 'F' )
            { 
                so_log::log::error("[so_imex::wav_module::import_audio] : unsupported format") ;
                return so_imex::sync_object::set_and_signal(
                    params_in.sync_ptr, so_imex::failed ) ;
            }

            if( ft_buffer[ 0 ] != 'W' || ft_buffer[ 1 ] != 'A' || ft_buffer[ 2 ] != 'V' )
            {
                so_log::log::error( "[so_imex::wav_module::import_audio] : unsupported format" ) ;
                return so_imex::sync_object::set_and_signal(
                    params_in.sync_ptr, so_imex::failed ) ;
            }
        }


        uint32_t const sib = uint32_t( *uint32_ptr_t( &data_buffer[ sizeof( uint32_t ) ] ) ) -
            sizeof( uint32_t ) ;
        
        wf.riff.size = sib ;
        
                
        {
            // strip of riff header
            size_t pos = sizeof( uint32_t ) * 3 ;

            so_std::string_t fmt_tmp = "xxxx" ;

            while( pos < sib )
            {
                size_t local_pos = pos ;

                this_file::chunk_t cur_chunk ;

                uint32_t const chunk_id = uint32_t( *uint32_ptr_t( &data_buffer[ local_pos ] ) ) ;
                local_pos += sizeof( uint32_t ) ; // id

                uint32_t const chunk_size = uint32_t( *uint32_ptr_t( &data_buffer[ local_pos ] ) ) ;
                local_pos += sizeof( uint32_t ) ; // size

                {
                    for( size_t i = 0; i < 4; ++i )
                    {
                        fmt_tmp[ i ] = ( chunk_id >> ( i << 3 ) ) & 255 ;
                        cur_chunk.id[ i ] = ( chunk_id >> ( i << 3 ) ) & 255 ;
                    }
                }

                // LIST
                if( fmt_tmp == "LIST" )
                {
                    wf.list.size = chunk_size ;
                }
                // "fmt " chunk : specifies the format of the data
                else if( fmt_tmp == "fmt " )
                {
                    uint16_t const chunk_afmt = uint16_t( *uint16_ptr_t( &data_buffer[ local_pos ] ) ) ;
                    local_pos += sizeof( uint16_t ) ; // audio format

                    uint16_t const chunk_chan = uint16_t( *uint16_ptr_t( &data_buffer[ local_pos ] ) ) ;
                    local_pos += sizeof( uint16_t ) ; // channels

                    uint32_t const samples_per_sec = uint32_t( *uint32_ptr_t( &data_buffer[ local_pos ] ) ) ;
                    local_pos += sizeof( uint32_t ) ; // samples per second

                    uint32_t const avg_bytes_per_sec = uint32_t( *uint32_ptr_t( &data_buffer[ local_pos ] ) ) ;
                    local_pos += sizeof( uint32_t ) ; // samples per second

                    uint16_t const block_align = uint16_t( *uint16_ptr_t( &data_buffer[ local_pos ] ) ) ;
                    local_pos += sizeof( uint16_t ) ; // block align

                    uint16_t const bits_per_sample = uint16_t( *uint16_ptr_t( &data_buffer[ local_pos ] ) ) ;
                    local_pos += sizeof( uint16_t ) ; // bits per sample

                    uint16_t const cb_size = uint16_t( *uint16_ptr_t( &data_buffer[ local_pos ] ) ) ;
                    local_pos += sizeof( uint16_t ) ; // extension size

                    uint16_t const valid_bits = uint16_t( *uint16_ptr_t( &data_buffer[ local_pos ] ) ) ;
                    local_pos += sizeof( uint16_t ) ; // valid bits per sample

                    uint32_t const dw_channel_mask = uint32_t( *uint32_ptr_t( &data_buffer[ local_pos ] ) ) ;
                    local_pos += sizeof( uint32_t ) ; // channel mask

                    wf.fmt.size = chunk_size ;
                    wf.fmt.format = chunk_afmt ;
                    wf.fmt.channels = chunk_chan ;
                    wf.fmt.samples_per_sec = samples_per_sec ;
                    wf.fmt.avg_bytes_per_sec = avg_bytes_per_sec ;
                    wf.fmt.block_align = block_align ;
                    wf.fmt.bits_per_sample = bits_per_sample ;
                    wf.fmt.ext_size = cb_size ;
                    wf.fmt.valid_bits_per_sample = valid_bits ;
                    wf.fmt.channel_mask = dw_channel_mask ;
                }
                else if( fmt_tmp == "data" )
                {
                    wf.data.size = chunk_size ;

                    // pcm
                    if( wf.fmt.format == 1 )
                    {
                        if( wf.fmt.bits_per_sample == 8 )
                        { 
                            so_typedefs( pcm_audio<int8_t>, pcm ) ;

                            size_t const num_samples = chunk_size / sizeof( uint8_t ) ;

                            pcm_t::init_data id ;
                            id.num_channels = wf.fmt.channels ;
                            id.num_samples_per_second = wf.fmt.samples_per_sec ;

                            pcm_t audio_data( id ) ;
                            audio_data.resize( num_samples ) ;

                            int8_ptr_t dptr = int8_ptr_t( &data_buffer[ local_pos ] ) ;
                            
                            for( size_t i = 0; i < num_samples; ++i )
                            {
                                size_t const rwi = i ;

                                pcm_t::sample_t s = { dptr[ rwi ] } ;
                                audio_data.set_sample( rwi, s ) ;
                            }
                            
                            // manage
                            {
                                audio_manager_t::manage_params_t mp ;
                                mp.data_ptr = pcm_t::create( std::move( audio_data ),
                                    "[wav_module::import_audio] : pcm_audio" ) ;
                                mp.file_path = params_in.path_to_file ;

                                auto const res = params_in.manager_reg_ptr->get_audio_manager()->insert(
                                    params_in.key, mp ) ;

                                if( res != so_imex::ok )
                                {
                                    so_log::log::error( "[wav_module::import_audio] : pcm_audio " +
                                        params_in.key ) ;

                                    mp.data_ptr->destroy() ;
                                }
                            }
                        }
                        else if( wf.fmt.bits_per_sample == 16 )
                        {
                            so_typedefs( pcm_audio<int16_t>, pcm ) ;

                            size_t const num_samples = chunk_size / sizeof( uint16_t ) ;

                            pcm_t::init_data id ;
                            id.num_channels = wf.fmt.channels ;
                            id.num_samples_per_second = wf.fmt.samples_per_sec ;

                            pcm_t audio_data( id ) ;
                            audio_data.resize( num_samples ) ;

                            int16_ptr_t dptr = int16_ptr_t( &data_buffer[ local_pos ] ) ;

                            for( size_t i = 0; i < num_samples; ++i )
                            {
                                size_t const rwi = i ;

                                pcm_t::sample_t s = { dptr[ rwi ] } ;
                                audio_data.set_sample( rwi, s ) ;
                            }
                            
                            // manage
                            {
                                audio_manager_t::manage_params_t mp ;
                                mp.data_ptr = pcm_t::create( std::move( audio_data ),
                                    "[wav_module::import_audio] : pcm_audio" ) ;
                                mp.file_path = params_in.path_to_file ;

                                auto const res = params_in.manager_reg_ptr->get_audio_manager()->insert(
                                    params_in.key, mp ) ;

                                if( res != so_imex::ok )
                                {
                                    so_log::log::error( "[wav_module::import_audio] : pcm_audio " +
                                        params_in.key ) ;

                                    mp.data_ptr->destroy() ;
                                }
                            }
                        }
                        else if( wf.fmt.bits_per_sample == 32 )
                        {
                            so_typedefs( pcm_audio<int32_t>, pcm ) ;

                            size_t const num_samples = chunk_size / sizeof( uint32_t ) ;

                            pcm_t::init_data id ;
                            id.num_channels = wf.fmt.channels ;
                            id.num_samples_per_second = wf.fmt.samples_per_sec ;

                            pcm_t audio_data( id ) ;
                            audio_data.resize( num_samples ) ;

                            int32_ptr_t dptr = int32_ptr_t( &data_buffer[ local_pos ] ) ;

                            for( size_t i = 0; i < num_samples; ++i )
                            {
                                size_t const rwi = i ;

                                pcm_t::sample_t s = { dptr[ rwi ] } ;
                                audio_data.set_sample( rwi, s ) ;
                            }

                            // manage
                            {
                                audio_manager_t::manage_params_t mp ;
                                mp.data_ptr = pcm_t::create( std::move( audio_data ), 
                                    "[wav_module::import_audio] : pcm_audio" ) ;
                                mp.file_path = params_in.path_to_file ;

                                auto const res = params_in.manager_reg_ptr->get_audio_manager()->insert(
                                    params_in.key, mp ) ;

                                if( res != so_imex::ok )
                                {
                                    so_log::log::error( "[wav_module::import_audio] : pcm_audio " + 
                                        params_in.key ) ;

                                    mp.data_ptr->destroy() ;
                                }
                            }
                        }
                        else
                        { 
                            so_log::log::error( "[so_imex::wav_module::import_audio] : "
                                "bits per sample not supported." ) ;
                        }
                    }
                    // ieee float
                    else if( wf.fmt.format == 3 )
                    {
                        if( wf.fmt.bits_per_sample == 32 )
                        {
                            so_typedefs( pcm_audio<float_t>, pcm ) ;

                            size_t const num_samples = chunk_size / sizeof( float_t ) ;

                            pcm_t::init_data id ;
                            id.num_channels = wf.fmt.channels ;
                            id.num_samples_per_second = wf.fmt.samples_per_sec ;

                            pcm_t audio_data( id ) ;
                            audio_data.resize( num_samples ) ;

                            float_ptr_t dptr = float_ptr_t( &data_buffer[ local_pos ] ) ;

                            for( size_t i = 0; i < num_samples; ++i )
                            {
                                size_t const rwi = i ;

                                pcm_t::sample_t s = { dptr[ rwi ] } ;
                                audio_data.set_sample( rwi, s ) ;
                            }

                            // manage
                            {
                                audio_manager_t::manage_params_t mp ;
                                mp.data_ptr = pcm_t::create( std::move( audio_data ),
                                    "[wav_module::import_audio] : pcm_audio" ) ;
                                mp.file_path = params_in.path_to_file ;

                                auto const res = params_in.manager_reg_ptr->get_audio_manager()->insert(
                                    params_in.key, mp ) ;

                                if( res != so_imex::ok )
                                {
                                    so_log::log::error( "[wav_module::import_audio] : pcm_audio " +
                                        params_in.key ) ;

                                    mp.data_ptr->destroy() ;
                                }
                            }
                        }
                        else
                        {
                            so_log::log::error( "[so_imex::wav_module::import_audio] : "
                                "bits per sample not supported." ) ;
                        }
                    }
                    else
                    {
                        so_log::log::error( "[so_imex::wav_module::import_audio] : "
                            "wav format not supported." ) ;
                    }
                }

                cur_chunk.size = chunk_size ;
                wf.chunks.push_back( cur_chunk ) ;

                // next chunk pos = cur_pos + chunk_id(4 bytes) + chunk_size(4 bytes)
                pos += chunk_size + ( sizeof( uint32_t ) << 1 ) ;
            }
        }

        //
        {}

        so_imex::sync_object_t::set_and_signal( params_in.sync_ptr, so_imex::ok ) ;

    } ) ;

    return so_thread::task_graph_t( import_task_ptr ) ;
}