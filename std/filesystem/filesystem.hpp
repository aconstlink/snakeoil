//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_STD_FILESYSTEM_FILESYSTEM_H_
#define _SNAKEOIL_STD_FILESYSTEM_FILESYSTEM_H_

#ifdef SNAKEOIL_COMPILER_MSC
#include <filesystem>
#elif SNAKEOIL_COMPILER_GNU
// requires -lstdc++fs on gcc
#include <experimental/filesystem>
#else
#error "do we have filesystem?" 
#endif

namespace so_std
{
#ifdef SNAKEOIL_COMPILER_MSC
    
    namespace filesystem = std::tr2::sys ;
    
#else
    
    namespace filesystem = std::experimental::filesystem ;
    
#endif
}

#endif
