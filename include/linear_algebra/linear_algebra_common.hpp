#pragma once

#include <type_traits>
#include <ostream>
#include <initializer_list>
#include <algorithm>
#include <numeric>
#include <vector>
#include <optional>
#include <variant>
#include <array>
#include <tuple>

#define LINEAR_ALGEBRA linear_algebra
#define NAMESPACE_LINEAR_ALGEBRA_BEGIN namespace LINEAR_ALGEBRA{
#define NAMESPACE_LINEAR_ALGEBRA_END }

#define LINEAR_ALGEBRA_IO linear_algebra_io
#define NAMESPACE_LINEAR_ALGEBRA_IO_BEGIN namespace LINEAR_ALGEBRA_IO{
#define NAMESPACE_LINEAR_ALGEBRA_IO_END }

NAMESPACE_LINEAR_ALGEBRA_BEGIN

//redeclaration of defined constants
constexpr bool use_openmp = USE_OPENMP;

//constant used to determine type of storage based on size of vector/matrix data
constexpr size_t static_storage_max_size = sizeof(double)*10000;

//type declaration
template<class T, size_t D>
class vector;

template<class T, size_t N, size_t M>
class matrix;

template<class... MS>
class matrix_multiplication_proxy;

enum class equation_system_type
{
    determinate,
    indeterminate,
    contradictory
};

template<class T, size_t M>
class equation_system_solution;

NAMESPACE_LINEAR_ALGEBRA_END