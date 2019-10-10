//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------

// automatically generated file - do not change
#include <snakeoil/std/typedefs.h>

#cmakedefine SNAKEOIL_COMPILER_MSC
#cmakedefine SNAKEOIL_COMPILER_MSC_14
#cmakedefine SNAKEOIL_COMPILER_MSC_15
#cmakedefine SNAKEOIL_COMPILER_GNU

#if defined( SNAKEOIL_COMPILER_MSC_14 )
#include <filesystem>
#elif defined( SNAKEOIL_COMPILER_MSC_15 )
#include <filesystem>
#elif SNAKEOIL_COMPILER_GNU
// requires -lstdc++fs on gcc
#include <experimental/filesystem>
#else
#error "do we have filesystem?" 
#endif

namespace so_std
{
    #ifdef SNAKEOIL_COMPILER_MSC_14

    namespace filesystem = std::tr2::sys ;

    #elif defined(SNAKEOIL_COMPILER_MSC_15)

    namespace filesystem = std::experimental::filesystem ;

    #elif defined( SNAKEOIL_COMPILER_GNU )

    namespace filesystem = std::experimental::filesystem ;

    #else
    #error "which filesystem to use"

    #endif
}


