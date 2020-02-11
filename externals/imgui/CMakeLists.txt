
set( IMGUI_INSTALL_HEADER_BASE "include/${PROJECT_NAME}/externals" )
add_subdirectory( imgui-1.7.5@30bb156 )
set( IMGUI_VERSION 1.7.5 )
set_target_properties( imgui PROPERTIES FOLDER "snakeoil/externals" )
set( IMGUI_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/imgui-1.7.5@30bb156/include" )

export( TARGETS imgui NAMESPACE snakeoil:: APPEND FILE ${SNAKEOIL_BINARY_DIR}/snakeoil-targets.cmake )
