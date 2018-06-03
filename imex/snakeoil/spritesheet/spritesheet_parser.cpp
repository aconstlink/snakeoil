//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "spritesheet_parser.h"

#include <snakeoil/std/string/split.hpp>
#include <snakeoil/memory/guards/malloc_guard.hpp>
#include <snakeoil/log/global.h>
#include <snakeoil/core/execute_if.hpp>

#include <rapidxml/rapidxml.hpp>

using namespace so_imex ;
using namespace so_imex::so_snakeoil ;

namespace this_file
{
   
    
}

//***********************************************************************
spritesheet_t spritesheet_parser::from_string( so_std::string_in_t din ) 
{
    so_memory::malloc_guard< char_t > din_tmp( din.c_str(), din.size()+1 ) ;
   
    rapidxml::xml_document<> doc ;
    doc.parse<0>( din_tmp.get() ) ;

    rapidxml::xml_node<> * root = doc.first_node( "spritesheet" ) ;
    if( so_core::is_nullptr(root) )
    {
        so_log::global_t::error( "[spritesheet_parser::from_string] : "
            "spritesheet xml need to start with <spritesheet>" ) ;

        return spritesheet_t() ;
    }

    spritesheet_t sso ;

    // check uri
    {
        rapidxml::xml_attribute<> * attr = root->first_attribute( "uri" ) ;
        if( so_core::is_nullptr( attr ) )
        {
            so_log::global_t::error( "[spritesheet_parser::from_string] : "
                "<spritesheet> requires uri" ) ;

            return spritesheet_t() ;
        }
        sso.uri = so_io::path_t( so_std::string_t( char_cptr_t( attr->value() ) ) ) ;
    }


    // go over sequences
    {
        rapidxml::xml_node<> * node = root->first_node( "sequences" ) ;
        if( so_core::is_nullptr( node ) )
        {
            so_log::global_t::error( "[spritesheet_parser::from_string] : "
                "<spritesheet> requires <sequences>" ) ;

            return spritesheet_t() ;
        }

        spritesheet_t::sequences_t seqs ;

        for( rapidxml::xml_node<> * n = node->first_node( "sequence" ); 
            so_core::is_not_nullptr(n); n = n->next_sibling() )
        {
            spritesheet_sequence_t seq ;

            // ATTR: check id
            {
                rapidxml::xml_attribute<> * attr = n->first_attribute( "id" ) ;
                if( so_core::is_nullptr( attr ) )
                {
                    so_log::global_t::error( "[spritesheet_parser::from_string] : "
                        "<sequence> requires id" ) ;

                    continue ;
                }

                seq.id = so_std::string_t( char_cptr_t( attr->value() ) ) ;
            }

            // ATTR: check speed
            {
                float_t speed = 1.0f ;

                rapidxml::xml_attribute<> * attr = n->first_attribute( "speed" ) ;
                if( so_core::is_not_nullptr( attr ) )
                {
                    speed = std::atof( attr->value() ) ;
                }

                seq.speed = speed ;
            }

            // NODE: frame
            {
                rapidxml::xml_node<> * n2 = n->first_node( "frame" ) ;
                if( so_core::is_nullptr( n2 ) )
                {
                    so_log::global_t::error( "[spritesheet_parser::from_string] : "
                        "<sequence> requires at least one <frame>" ) ;

                    continue ;
                }

                while( so_core::is_not_nullptr(n2) )
                {
                    spritesheet_frame_t frm ;

                    // ATTR: check rect
                    {
                        rapidxml::xml_attribute<> * attr = n2->first_attribute( "rect" ) ;
                        if( so_core::is_nullptr( attr ) )
                        {
                            break ;
                        }
                        
                        so_std::vector< so_std::string > values ;
                        so_std::string_ops::split( so_std::string_t( char_cptr_t( attr->value() ) ), ' ', values ) ;

                        for( size_t i=0; i<values.size(); ++i )
                        {
                            uint_t const v = uint_t( atoi( values[i].c_str() ) ) ;
                            frm.rect[ i ] = v ;
                        }
                    }

                    // ATTR: check duration
                    {
                        rapidxml::xml_attribute<> * attr = n2->first_attribute( "duration" ) ;
                        if( so_core::is_nullptr( attr ) )
                        {
                            continue ;
                        }
                        uint_t const dur = uint_t( std::atoi( attr->value() ) ) ;

                        frm.duration = dur ;
                    }

                    // NODE: hit_zones
                    {
                        rapidxml::xml_node<> * n3 = n2->first_node( "hit_zones" ) ;
                        if( so_core::is_nullptr( n3 ) )
                        {
                            so_log::global_t::error( "[spritesheet_parser::from_string] : "
                                "<frame> requires one <hit_zones>" ) ;

                            continue ;
                        }

                        rapidxml::xml_node<> * n4 = n3->first_node() ;
                        if( so_core::is_nullptr( n4 ) )
                        {
                            so_log::global_t::error( "[spritesheet_parser::from_string] : "
                                "<frame> requires at least one hit zone" ) ;

                            continue ;
                        }

                        // cycle through all hit zones
                        while( so_core::is_not_nullptr(n4) )
                        {
                            spritesheet_hitzone_t hz ;

                            if( strcmp( n4->name(), "rect" ) == 0 )
                            {
                                hz.t = spritesheet_hitzone_t::type::rect ;
                            }
                            else if( strcmp( n4->name(), "circle" ) == 0 )
                            {
                                hz.t = spritesheet_hitzone_t::type::circle ;
                            }
                            else
                            {
                                break ;
                            }

                            // ATTR: values of hit zone
                            {
                                rapidxml::xml_attribute<> * attr = n4->first_attribute( "values" ) ;
                                if( so_core::is_nullptr( attr ) )
                                {
                                    break ;
                                }
                                uint_t const dur = uint_t( std::atoi( attr->value() ) ) ;
                                
                                so_std::vector< so_std::string > values ;
                                so_std::string_ops::split( so_std::string_t( char_cptr_t( attr->value() ) ), ' ', values ) ;

                                for( size_t i = 0; i < values.size(); ++i )
                                {
                                    uint_t const v = uint_t( atoi( values[ i ].c_str() ) ) ;
                                    hz.values[ i ] = v ;
                                }
                            }

                            frm.hitzones.push_back( hz ) ;

                            n4 = n4->next_sibling() ;
                        }
                    }

                    seq.frames.push_back( std::move( frm ) ) ;

                    n2 = n2->next_sibling() ;
                }
            }

            sso.sequences.push_back( std::move(seq) ) ;
        }
    }

    


    return std::move( sso ) ;
}

//***********************************************************************
so_std::string_t spritesheet_parser::from_data( spritesheet_in_t ) 
{
    return so_std::string_t() ;
}