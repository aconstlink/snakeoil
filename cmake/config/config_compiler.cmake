#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# determine the compiler
# for cross compilation, the target platform is required to be known
# 
# the values here can be initialized using a toolchain file

set( SNAKEOIL_COMPILER_CONFIGURED FALSE )

# Microsoft compiler
set( SNAKEOIL_COMPILER_MSC OFF )
# Gnu compiler (GCC, GCC-C++)
set( SNAKEOIL_COMPILER_GNU OFF )
set( SNAKEOIL_COMPILER_CLANG OFF )

if( MSVC_IDE OR MSVC )

    set( SNAKEOIL_COMPILER_MSC ON )
    #add_definitions( -DSNAKEOIL_COMPILER_MSC )
    #set( SNAKEOIL_COMPILER_DEFINITION_PUBLIC SNAKEOIL_COMPILER_MSC )

elseif( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )

    set( SNAKEOIL_COMPILER_GNU ON )
    #add_definitions( -DSNAKEOIL_COMPILER_GNU )
else()

    message( FATAL_ERROR "Unsupported compiler")

endif()

#
# Print Info
#
if( SNAKEOIL_COMPILER_MSC )

    message( STATUS "[compiler] : Microsoft Compiler Suite" )

elseif( SNAKEOIL_COMPILER_GNU )

    message( STATUS "[compiler] : GNU Compiler Suite" )

endif()

set( SNAKEOIL_COMPILER_CONFIGURED TRUE )

