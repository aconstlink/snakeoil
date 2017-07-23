#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( CMAKE_AUTOMOC ON )

# required for including binary path for auto generated files
# like moc files
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add the include directories for the Qt 5 modules to
# the compile lines.
include_directories( ${Qt5Core_INCLUDE_DIRS} )
include_directories( ${Qt5Widgets_INCLUDE_DIRS} )

# Use the compile definitions defined in the Qt 5 module
add_definitions( ${Qt5Core_DEFINITIONS} )
add_definitions( ${Qt5Widgets_DEFINITIONS} )

set( SOURCES 
	result.h
	typedefs.h
	protos.h
	meta_types.h
	timeline/timeline.h
	timeline/timeline.cpp
	timeline/view/timeline_view.h
	timeline/view/timeline_view.cpp
	timeline/scrollbar/timeline_scrollbar.h
	timeline/scrollbar/timeline_scrollbar.cpp
	
	time_converter/time_converter.h
	time_converter/time_converter.cpp
	timer/timer.h
	timer/timer.cpp
	
	player/player.h
	player/player.cpp
	
	property/variant_manager.h
	property/variant_manager.cpp
	
	asset_manager/shader_types.h
	asset_manager/asset_manager.h
	asset_manager/asset_manager.cpp
	asset_manager/items/script_items.h
	asset_manager/items/script_items.cpp
	asset_manager/items/shader_items.h
	asset_manager/items/shader_items.cpp
	
	clock/countdown_clock.h
	clock/countdown_clock.cpp
	)

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} STATIC ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )