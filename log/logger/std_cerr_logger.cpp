//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "std_cerr_logger.h"

#include <iostream>

using namespace so_log ;

//*************************************************************************************
std_cerr_logger::std_cerr_logger( void_t ) 
{

}

//*************************************************************************************
std_cerr_logger::~std_cerr_logger( void_t ) 
{

}

//*************************************************************************************
so_log::result std_cerr_logger::log( so_log::log_level ll, std::string const & msg ) 
{
    so_thread::lock_t lk(_mtx) ;
    std::cerr << so_log::to_string_short(ll) << ((ll == so_log::log_level::raw) ? "" : " ") << msg << std::endl ;
    return so_log::ok ;
}

