//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_WAV_WAV_MODULE_H_
#define _SNAKEOIL_IMEX_MODULE_WAV_WAV_MODULE_H_

#include "../iaudio_module.h"

namespace so_imex
{
    class SNAKEOIL_IMEX_API wav_module : public iaudio_module
    {
        so_this_typedefs( wav_module ) ;

    public:

        wav_module( void_t ) ;
        wav_module( this_cref_t ) = delete ;
        wav_module( this_rref_t ) ;
        virtual ~wav_module( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    private:


        virtual so_imex::result initialize( init_params_cref_t ) ;

    public:

        virtual bool_t create_properties( property_map_inout_t ) const ;

        virtual bool_t is_format_supported( so_std::string_cref_t ) const ;
        virtual so_imex::file_extension_names_t get_file_extension_names( void_t ) const ;

        virtual so_thread::task_graph_t import_audio( import_params_cref_t ) ;
        virtual so_thread::task_graph_t export_audio( export_params_cref_t ) ;

    public:

        virtual so_imex::result destroy( void_t ) ;
    };
    so_typedef( wav_module ) ;
}

#endif