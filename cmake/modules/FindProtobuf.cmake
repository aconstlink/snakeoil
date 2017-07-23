#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# Finds the Protocol Buffers library
#

set( PROTOBUF_FOUND False )

#
# SECTION: User option
#
if( NOT PROTOBUF_ENABLE )
    set( PROTOBUF_ENABLE off )
endif()

option( PROTOBUF_ENABLE "Enable Google protobuf" ${PROTOBUF_ENABLE} )

if( PROTOBUF_ENABLE )
	message( STATUS "[Enabled] : Google protobuf" )
else()
	message( STATUS "[Disabled] : Google protobuf" )
	return()
endif()

set( PROTOBUF_DEFINITIONS 
	-DPROTOBUF_ENABLE
	)

# unfortunately required by user of protobuf
set( PROTOBUF_DISABLE_WARNINGS_MSC "/wd4512 /wd4244 /wd4267 /wd4125 /wd4996" )

#
# SECTION: Include path
#

find_path( PROTOBUF_INCLUDE_DIRECTORY 
	NAMES "google/protobuf" 
	HINTS $ENV{PROTOBUF_INCLUDEDIR} 
)

if( NOT PROTOBUF_INCLUDE_DIRECTORY )
	message( FATAL_ERROR "ProtoBuf include dir not found." )
endif() 

set( PROTOBUF_INCLUDE_DIRS 
	${PROTOBUF_INCLUDE_DIRECTORY} 
)

#
# SECTION: Library paths
#

find_library( PROTOBUF_LIBRARY_PROTOBUF_DEBUG 
	NAMES "protobuf" "libprotobuf"
	HINTS $ENV{PROTOBUF_LIBRARYDIR}/debug
	)
	
find_library( PROTOBUF_LIBRARY_PROTOBUF_RELEASE 
	NAMES "protobuf" "libprotobuf"
	HINTS $ENV{PROTOBUF_LIBRARYDIR}/release
	)

find_library( PROTOBUF_LIBRARY_PROTOBUF_LITE_DEBUG 
	NAMES "protobuf-lite" "libprotobuf-lite"
	HINTS $ENV{PROTOBUF_LIBRARYDIR}/debug
	)

find_library( PROTOBUF_LIBRARY_PROTOBUF_LITE_RELEASE 
	NAMES "protobuf-lite" "libprotobuf-lite"
	HINTS $ENV{PROTOBUF_LIBRARYDIR}/release
	)

find_library( PROTOBUF_LIBRARY_PROTOBUF_PROTOC_DEBUG 
	NAMES "protoc" "libprotoc"
	HINTS $ENV{PROTOBUF_LIBRARYDIR}/debug
	)

find_library( PROTOBUF_LIBRARY_PROTOBUF_PROTOC_RELEASE 
	NAMES "protoc" "libprotoc"
	HINTS $ENV{PROTOBUF_LIBRARYDIR}/release
	)

if( NOT PROTOBUF_LIBRARY_PROTOBUF_DEBUG )
	message( FATAL_ERROR "Protobuf Library not found" )
endif()

if( NOT PROTOBUF_LIBRARY_PROTOBUF_RELEASE )
	message( FATAL_ERROR "Protobuf Library not found" )
endif()

if( NOT PROTOBUF_LIBRARY_PROTOBUF_LITE_DEBUG )
	message( FATAL_ERROR "Protobuf Library not found" )
endif()

if( NOT PROTOBUF_LIBRARY_PROTOBUF_LITE_RELEASE )
	message( FATAL_ERROR "Protobuf Library not found" )
endif()
	
if( NOT PROTOBUF_LIBRARY_PROTOBUF_PROTOC_DEBUG )
	message( FATAL_ERROR "Protobuf Library not found" )
endif()

if( NOT PROTOBUF_LIBRARY_PROTOBUF_PROTOC_RELEASE )
	message( FATAL_ERROR "Protobuf Library not found" )
endif()

set( PROTOBUF_LITE_LIBRARIES
	debug ${PROTOBUF_LIBRARY_PROTOBUF_LITE_DEBUG}
	optimized ${PROTOBUF_LIBRARY_PROTOBUF_LITE_RELEASE}
	)
	
set( PROTOBUF_LIBRARIES
	debug ${PROTOBUF_LIBRARY_PROTOBUF_DEBUG}
	optimized ${PROTOBUF_LIBRARY_PROTOBUF_RELEASE}
	)

set( PROTOC_LIBRARIES
	debug ${PROTOBUF_LIBRARY_PROTOBUF_PROTOC_DEBUG}
	optimized ${PROTOBUF_LIBRARY_PROTOBUF_PROTOC_RELEASE}
	)	
	
message( STATUS "[Found] : ProtoBuf" )


set( PROTOBUF_FOUND True )
