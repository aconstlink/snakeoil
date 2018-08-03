//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../../api.h"
#include "../../typedefs.h"


#include <snakeoil/io/typedefs.h>
#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/math/vector/vector4.hpp>
namespace so_imex
{
    namespace so_snakeoil
    {
        //***********************************************************
        struct spritesheet_hitzone
        {

            enum class type 
            {
                rect,
                circle
            };


            type t ;
            so_math::vec4ui_t values ;
        };
        so_typedef( spritesheet_hitzone ) ;

        //***********************************************************
        struct spritesheet_frame
        {
            so_this_typedefs( spritesheet_frame ) ;

            so_math::vec4ui_t rect ;
            uint_t duration ;

            so_typedefs( so_std::vector< spritesheet_hitzone >, spritesheet_hitzones ) ;
            spritesheet_hitzones_t hitzones ;

            spritesheet_frame( void_t ) {}
            spritesheet_frame( this_cref_t ) = delete ;
            spritesheet_frame( this_rref_t rhv ) 
            {
                *this = std::move(rhv) ;
            }

            ~spritesheet_frame(void_t ) {}

            this_ref_t operator = ( this_rref_t rhv )
            {
                rect = rhv.rect ;
                duration = rhv.duration ;
                hitzones = std::move( rhv.hitzones ) ;
                return *this ;
            }
        };
        so_typedef( spritesheet_frame ) ;

        //***********************************************************
        struct spritesheet_sequence
        {
            so_this_typedefs( spritesheet_sequence ) ;

            so_typedefs( so_std::vector< spritesheet_frame >, frames ) ;
            frames_t frames ;

            so_std::string_t id ;
            float_t speed ;



            spritesheet_sequence( void_t ) {}
            spritesheet_sequence( this_cref_t ) = delete ;
            spritesheet_sequence( this_rref_t rhv ) {
                *this = std::move( rhv ) ;
            }
            ~spritesheet_sequence( void_t ){}

            this_ref_t operator = ( this_rref_t rhv ) 
            {
                id = std::move( rhv.id ) ;
                speed = rhv.speed ;
                frames = std::move( rhv.frames ) ;
                return *this ;
            }

        };
        so_typedef( spritesheet_sequence ) ;

        //***********************************************************
        struct spritesheet
        {
            so_this_typedefs( spritesheet ) ;

            so_io::path_t uri ;

            so_typedefs( so_std::vector< spritesheet_sequence >, sequences ) ;
            sequences_t sequences ;


            spritesheet( void_t ) {}
            spritesheet( this_cref_t ) = delete ;
            spritesheet( this_rref_t rhv ) 
            {
                *this = std::move( rhv ) ;
            }

            ~spritesheet( void_t ) {}

            this_ref_t operator = ( this_rref_t rhv )
            {
                uri = std::move( rhv.uri ) ;
                sequences = std::move( rhv.sequences ) ;

                return *this ;
            }
        };
        so_typedef( spritesheet ) ;
    }
}