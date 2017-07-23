#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# @VAR_NAME pass the variable name that might contain the lib path
# @LIB_NAME the libraries name for message output reason on fail
#
macro( so_lib_assert VAR_NAME LIB_NAME )
	#message( "In macro " ${${VAR_NAME}} )
	if( NOT ${VAR_NAME} )
		message( "*** Looking for " ${LIB_NAME} )
		message( "Verzerrzt Library Path CMake is looking in: " ${SNAKEOIL_LIBRARIES} )
		message( FATAL_ERROR "Please check content of SNAKEOIL_LIBRARIES." )
	endif()
endmacro()

