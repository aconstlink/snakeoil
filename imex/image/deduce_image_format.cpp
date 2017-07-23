//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "deduce_image_format.h"

#include <snakeoil/std/string/split.hpp>

using namespace so_imex ;

//*************************************************************************************
image_file_format deduce_image_format::from_path( so_io::path_cref_t p ) 
{
    so_std::vector<so_std::string_t> tokens ;
    so_std::string_ops::split( so_std::string_t( p.string() ), '.', tokens ) ;
        
    if( tokens.size() == 0 )
        return so_imex::image_file_format::undefined ;

    // file format string
    std::string ffs = std::move(*(--tokens.end())) ;

    if( ffs == "jpg" || ffs == "jpeg") return so_imex::image_file_format::jpg ;
    else if( ffs == "bmp" ) return so_imex::image_file_format::bmp ;
    else if( ffs == "png" ) return so_imex::image_file_format::png ;
    else if( ffs == "gif" ) return so_imex::image_file_format::gif ;
    else if( ffs == "hdr" ) return so_imex::image_file_format::hdr ;
    
    // more here 
    else return so_imex::image_file_format::undefined ;
}

//*************************************************************************************
std::string deduce_image_format::from_format( so_imex::image_file_format if_in ) 
{
    switch( if_in )
    {
    case so_imex::image_file_format::bmp : return std::string("bmp") ;
    case so_imex::image_file_format::jpg : return std::string("jpg") ;
    case so_imex::image_file_format::gif : return std::string("gif") ;
    case so_imex::image_file_format::png : return std::string("png") ;
    case so_imex::image_file_format::hdr : return std::string("hdr") ;
    default: return std::string( std::string("") ) ;
    }

    return std::string("") ;
}
