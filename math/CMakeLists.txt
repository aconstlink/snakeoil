#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( CPP_SOURCES 
    common/cpp/fma.hpp
    common/cpp/function.hpp
)

set( AVX_SOURCES )

# not arch specific
set( NON_ARCH_SOURCES
    common/fast.hpp

    curve/catmull_rom_curve.hpp
    curve/cubic_bezier_curve.hpp
    
    color/protos.hpp
    color/color3.hpp
    color/color4.hpp
    color/switch.h

    vector/switch.h
    vector/protos.hpp
    vector/vector2.hpp
    vector/vector3.hpp
    vector/vector4.hpp
    vector/vector2b.hpp
    vector/vector3b.hpp
    vector/vector4b.hpp

    
    interpolation/interpolate.hpp
        
    matrix/switch.h
    matrix/protos.hpp
    matrix/matrix2.hpp
    matrix/matrix3.hpp
    matrix/matrix4.hpp
    quaternion/protos.hpp
    quaternion/quaternion3.hpp
    quaternion/quaternion4.hpp

    euler/euler_angles.hpp
    euler/euler_sequence.h
        
    planar/plane/hesse_plane.hpp
    planar/plane/hesse_plane3.hpp
    planar/plane/plane.hpp

    primitive/2d/line.hpp
    primitive/2d/ray.hpp
    primitive/2d/line_segment.hpp
    primitive/2d/ray_line_intersect.hpp
    primitive/3d/ray.hpp
    primitive/3d/frustum.hpp

    utility/radian.hpp
    utility/degree.hpp
    utility/angle.hpp
    utility/fn.hpp
    utility/index_1d_to_2d.hpp
    utility/index_2d_to_1d.hpp
    utility/index_3d_to_1d.hpp

    utility/3d/look_at.hpp
    utility/3d/ortho_basis.hpp
    utility/3d/perspective_fov.hpp
    utility/3d/orthographic_projection.hpp
    utility/3d/transformation.hpp


    utility/2d/viewport.hpp
    utility/2d/coord_transformator.hpp
    utility/2d/index_transformator.hpp
    utility/2d/transformation.hpp

    camera/3d/perspective_camera.hpp
    camera/3d/orthographic_camera.hpp
)

# compound include files
set( SOURCES 
    default_instruction_set.h
    typedefs.h
    common/function.h
    constants.h

    ${CPP_SOURCES}
    ${NON_ARCH_SOURCES}	
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

