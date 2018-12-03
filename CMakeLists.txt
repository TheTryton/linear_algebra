cmake_minimum_required(VERSION 3.8)
project(linear_algebra)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(linear_algebra_Sources
	main.cpp
	
	include/linear_algebra.hpp
	include/common_matrix_vector_operations.inl
	
	include/matrix/matrix.hpp
	include/vector/vector.hpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${linear_algebra_Sources})

add_library(linear_algebra STATIC ${linear_algebra_Sources})

target_include_directories(linear_algebra PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(linear_algebra PROPERTIES CXX_STANDARD 17)
set_target_properties(linear_algebra PROPERTIES LINKER_LANGUAGE CXX)