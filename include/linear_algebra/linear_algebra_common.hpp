#pragma once

#include <type_traits>
#include <ostream>
#include <initializer_list>
#include <optional>
#include <algorithm>
#include <numeric>
#include <vector>

#define LINEAR_ALGEBRA linear_algebra
#define NAMESPACE_LINEAR_ALGEBRA_BEGIN namespace LINEAR_ALGEBRA{
#define NAMESPACE_LINEAR_ALGEBRA_END }

NAMESPACE_LINEAR_ALGEBRA_BEGIN

template<class T, size_t D>
class vector;

template<class T, size_t N, size_t M>
class matrix;

constexpr bool use_high_quality_equality_comparison = false;

template<class T, class TO>
inline bool equal(const T& a, const TO& b)
{
    if constexpr (use_high_quality_equality_comparison)
    {
        return std::abs(a - b) <= std::numeric_limits<decltype(a - b)>::epsilon();
    }
    if constexpr (!use_high_quality_equality_comparison)
    {
        return a == b;
    }
}

template<class T, size_t M>
using equation_system_solution = std::pair<vector<T, M>, std::vector<vector<T, M>>>;

NAMESPACE_LINEAR_ALGEBRA_END