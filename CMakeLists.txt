cmake_minimum_required(VERSION 3.8)
project(linear_algebra)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(linear_algebra_Sources
    main.cpp

    ##linear_algebra

    include/linear_algebra/linear_algebra_common.hpp

    include/linear_algebra/matrix/matrix.hpp
    include/linear_algebra/vector/vector.hpp

    include/linear_algebra/common_matrix_vector_operations.inl

    include/linear_algebra/equation_system/equation_system.hpp

    include/linear_algebra/linear_algebra.hpp

    ##n-dimensional geometry

    include/geometry/geometry_common.hpp

    include/geometry/primitives/space.hpp
    include/geometry/primitives/space.inl
    include/geometry/primitives/simplex.hpp
    include/geometry/primitives/simplex.inl
    include/geometry/primitives/sphere.hpp
    include/geometry/primitives/sphere.inl

    include/geometry/projections/projections.hpp
    include/geometry/projections/projections.inl

    include/geometry/intersections/intersections.hpp
    include/geometry/intersections/intersections.inl

    include/geometry/generalized_vector_product/generalized_vector_product.hpp
    include/geometry/generalized_vector_product/generalized_vector_product.inl

    ##2D geometry

    include/geometry2D/geometry2D_common.hpp

    include/geometry2D/primitives/line.hpp
    include/geometry2D/primitives/line.inl
    include/geometry2D/primitives/segment.hpp
    include/geometry2D/primitives/segment.inl
    include/geometry2D/primitives/line_segment_common.inl
    include/geometry2D/primitives/rect.hpp
    include/geometry2D/primitives/rect.inl
    include/geometry2D/primitives/triangle.hpp
    include/geometry2D/primitives/triangle.inl

    include/geometry2D/intersections/intersections.hpp
    include/geometry2D/intersections/intersections.inl

    include/geometry2D/projections/projections.hpp
    include/geometry2D/projections/projections.inl

    include/geometry2D/geometry2D_io.hpp
    include/geometry2D/geometry2D.hpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${linear_algebra_Sources})

add_executable(linear_algebra ${linear_algebra_Sources})

target_include_directories(linear_algebra PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(linear_algebra PROPERTIES CXX_STANDARD 17)
set_target_properties(linear_algebra PROPERTIES LINKER_LANGUAGE CXX)