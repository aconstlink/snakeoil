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
#include <rapidxml/rapidxml_print.hpp>

#include <iostream>
#include <sstream>

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
            so_log::global_t::warning( "[spritesheet_parser::from_string] : "
                "<spritesheet> has no <sequences>" ) ;

            return std::move( sso ) ;
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
                            so_log::global_t::error( "[spritesheet_parser::from_string] : "
                                "<frame> requires a duration" ) ;

                            n2 = n2->next_sibling() ;
                            continue ;
                        }
                        uint_t const dur = uint_t( std::atoi( attr->value() ) ) ;

                        frm.duration = dur ;
                    }

                    // NODE: hit_zones
                    {
                        rapidxml::xml_node<> * n3 = n2->first_node( "hit_zones" ) ;
                        if( so_core::is_not_nullptr( n3 ) )
                        {
                            rapidxml::xml_node<> * n4 = n3->first_node() ;
                            if( so_core::is_nullptr( n4 ) )
                            {
                                so_log::global_t::warning( "[spritesheet_parser::from_string] : "
                                    "<hit_zones> is empty" ) ;
                            }

                            // cycle through all hit zones
                            while( so_core::is_not_nullptr( n4 ) )
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
so_std::string_t spritesheet_parser::from_data( spritesheet_in_t ssi ) 
{

    rapidxml::xml_document<> doc;
    
    /*rapidxml::xml_node<> *node = doc.allocate_node( rapidxml::node_element, "a", "Google" );

    doc.append_node( node );
     rapidxml::xml_attribute<> *attr = doc.allocate_attribute( "href", "google.com" );
    node->append_attribute( attr );*/

    rapidxml::xml_node<> * spritesheet_node = doc.allocate_node( rapidxml::node_element, "spritesheet" );
    doc.append_node( spritesheet_node );

    // ATTR: uri
    {
        char * s = doc.allocate_string( ssi.uri.string().c_str() ) ;
        rapidxml::xml_attribute<> *attr = doc.allocate_attribute( "uri", s );
        spritesheet_node->append_attribute( attr );
    }

    // NODE: sequences
    {
        rapidxml::xml_node<> * sequences_node = doc.allocate_node( rapidxml::node_element, "sequences" );
        for( auto & seq : ssi.sequences )
        {
            rapidxml::xml_node<> * sequence_node = doc.allocate_node( rapidxml::node_element, "sequence" );

            // ATTR: id
            {
                char * s = doc.allocate_string( seq.id.c_str() ) ;
                rapidxml::xml_attribute<> *attr = doc.allocate_attribute( "id", s ) ;
                sequence_node->append_attribute( attr );
            }

            // ATTR: speed
            {
                char * s = doc.allocate_string( std::to_string(seq.speed).c_str() ) ;
                rapidxml::xml_attribute<> *attr = doc.allocate_attribute( "speed", s ) ;
                sequence_node->append_attribute( attr );
            }

            // NODE: frame
            for( auto & frame : seq.frames )
            {
                rapidxml::xml_node<> * frame_node = doc.allocate_node( rapidxml::node_element, "frame" ) ;

                // ATTR: rect
                {
                    char * s = doc.allocate_string( 
                        (std::to_string( frame.rect.x() )+" "+std::to_string( frame.rect.y() )+ " " +
                            std::to_string( frame.rect.z() )+" "+std::to_string( frame.rect.w() )).c_str() ) ;
                    rapidxml::xml_attribute<> *attr = doc.allocate_attribute( "rect", s ) ;
                    frame_node->append_attribute( attr );
                }

                // ATTR: duration
                {
                    char * s = doc.allocate_string( std::to_string(frame.duration).c_str() ) ;
                    rapidxml::xml_attribute<> *attr = doc.allocate_attribute( "duration", s ) ;
                    frame_node->append_attribute( attr );
                }

                // NODE: hit_zones
                {
                    rapidxml::xml_node<> * hitzones_node = doc.allocate_node( rapidxml::node_element, "hit_zones" ) ;

                    for( auto & hz : frame.hitzones )
                    {
                        rapidxml::xml_node<> * hz_node = nullptr ;

                        so_std::string_t values ;

                        if( hz.t == so_imex::so_snakeoil::spritesheet_hitzone::type::rect )
                        {
                            hz_node = doc.allocate_node( rapidxml::node_element, "rect" ) ;
                            values = std::to_string( hz.values.x() ) + " " + std::to_string(  hz.values.y() ) + " " +
                                std::to_string(  hz.values.z() ) + " " + std::to_string(  hz.values.w() ) ;
                        }
                        else if( hz.t == so_imex::so_snakeoil::spritesheet_hitzone::type::circle )
                        {
                            hz_node = doc.allocate_node( rapidxml::node_element, "circle" ) ;
                            values = std::to_string( hz.values.x() ) + " " + std::to_string( hz.values.y() ) + " " +
                                std::to_string( hz.values.z() ) + " " + std::to_string( hz.values.w() ) ;
                        }

                        if( so_core::is_not_nullptr(hz_node) )
                        {
                            // ATTR: values
                            {
                                char * s = doc.allocate_string( values.c_str() ) ;
                                rapidxml::xml_attribute<> *attr = doc.allocate_attribute( "values", s ) ;
                                hz_node->append_attribute( attr );
                            }

                            hitzones_node->append_node( hz_node ) ;
                        }
                    }

                    frame_node->append_node( hitzones_node ) ;
                }

                sequence_node->append_node( frame_node ) ;
            }

            sequences_node->append_node( sequence_node ) ;
        }
        spritesheet_node->append_node( sequences_node ) ;
    }

    std::stringstream ss ;
    ss << doc ;

    return ss.str() ;
}