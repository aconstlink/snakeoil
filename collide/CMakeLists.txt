#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES 
    typedefs.h

    hit_test_types.h

    3d/hit_test/hit_test_aabb_aabb.hpp
    3d/hit_test/hit_test_aabb_frustum.hpp
    3d/hit_test/hit_test_aabb_plane.hpp
    3d/hit_test/hit_test_aabb_sphere.hpp
    3d/hit_test/hit_test_aabb_point.hpp
    3d/hit_test/hit_test_aabb_ray.hpp
    3d/hit_test/hit_test_plane_ray.hpp

    3d/bounds/aabb.hpp
    3d/bounds/sphere.hpp
    3d/bounds/view_frustum.hpp
        
    2d/hit_test/hit_test_aabb_aabb.hpp
    2d/hit_test/hit_test_aabb_frustum.hpp
    2d/hit_test/hit_test_aabb_circle.hpp
    2d/hit_test/hit_test_aabb_ray.hpp
    2d/hit_test/hit_test_aabb_line_segment.hpp

    2d/collision/circle_line.hpp

    2d/bounds/aabb.hpp
    2d/bounds/circle.hpp
    2d/bounds/obb.hpp
    )

so_vs_src_dir( SOURCES )

#add_custom_target( ${CUR_LIB_NAME} SOURCES ${SOURCES} )	
add_library( ${CUR_LIB_NAME} STATIC ${SOURCES} )
set_target_properties( ${CUR_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )
