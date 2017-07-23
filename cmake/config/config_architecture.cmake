#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# determine/option the host and target architecture
#
# the values here can be initialized using a toolchain file

set( SNAKEOIL_ARCH_CONFIGURED FALSE )

set( SNAKEOIL_HOST_ARCH "Unknown" )
set( SNAKEOIL_HOST_ARCH_X86 OFF )
set( SNAKEOIL_HOST_ARCH_BIT_32 OFF )
set( SNAKEOIL_HOST_ARCH_BIT_64 OFF )


set( SNAKEOIL_TARGET_ARCH "Unknown" )
set( SNAKEOIL_TARGET_ARCH_X86 ON ) # default
set( SNAKEOIL_TARGET_ARCH_ARM OFF )
set( SNAKEOIL_TARGET_ARCH_CBEA OFF )
set( SNAKEOIL_TARGET_ARCH_PPC OFF )
set( SNAKEOIL_TARGET_ARCH_MIPS OFF )
set( SNAKEOIL_TARGET_ARCH_BIT_32 OFF )
set( SNAKEOIL_TARGET_ARCH_BIT_64 ON ) #default

set( SNAKEOIL_ARCH_CONFIGURED TRUE )

