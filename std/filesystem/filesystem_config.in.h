//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------

// automatically generated file - do not change
#include <snakeoil/std/typedefs.h>

#cmakedefine SNAKEOIL_COMPILER_MSC
#cmakedefine SNAKEOIL_COMPILER_GNU

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


