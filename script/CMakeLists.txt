#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES ${SOURCES}
    api.h
    typedefs.h
    protos.h
    result.h
)

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} ${SNAKEOIL_LIBRARY_BUILD_TYPE} ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

if( SNAKEOIL_LIBRARY_BUILD_SHARED )

	add_definitions( 
		-DSNAKEOIL_SCRIPT_EXPORT 
		#-DSNAKEOIL_GPX_IMPORT
		#-DSNAKEOIL_GPU_IMPORT
		-DSNAKEOIL_MEMORY_IMPORT
		-DSNAKEOIL_LOG_IMPORT
		)
	
	# snakeoil
	target_link_libraries( ${CUR_LIB_NAME}
		#snakeoil_gpx
		#snakeoil_gpu
		snakeoil_memory
		snakeoil_log
	)
	
endif()

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )

