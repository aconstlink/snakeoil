#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
#
#

set( SNAKEOIL_REAL_TYPE_CONFIGURED FALSE )

set( SNAKEOIL_REAL_TYPE "float" CACHE STRING "Select the to be used floating point precision type." )
set_property(CACHE SNAKEOIL_REAL_TYPE PROPERTY STRINGS "float" "double")

if( SNAKEOIL_REAL_TYPE STREQUAL "float" )
	add_definitions( -DSNAKEOIL_REAL_FLOAT )
elseif( SNAKEOIL_REAL_TYPE STREQUAL "double" )
	add_definitions( -DSNAKEOIL_REAL_DOUBLE )
else()
	add_definitions( -DSNAKEOIL_REAL_FLOAT )
endif()

message( STATUS "[done] : Real Type" )

set( SNAKEOIL_REAL_TYPE_CONFIGURED TRUE )

