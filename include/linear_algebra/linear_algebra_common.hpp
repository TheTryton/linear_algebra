#pragma once

#include <type_traits>
#include <ostream>
#include <initializer_list>
#include <algorithm>
#include <numeric>
#include <vector>
#include <optional>
#include <variant>

#define LINEAR_ALGEBRA linear_algebra
#define NAMESPACE_LINEAR_ALGEBRA_BEGIN namespace LINEAR_ALGEBRA{
#define NAMESPACE_LINEAR_ALGEBRA_END }

NAMESPACE_LINEAR_ALGEBRA_BEGIN

//redeclaration of defined constants

constexpr bool use_openmp = USE_OPENMP;

//specializable struct used for overloading commonly used functions for non-standard types
template<class T>
struct functions_implementation
{
    static inline T sqrt(T v)
    {
        return T();
    }

    static inline T abs(T v)
    {
        return v > static_cast<T>(0) ? v : static_cast<T>(-1) * v;
    }

    static constexpr inline T epsilon()
    {
        return std::numeric_limits<T>::epsilon();
    }
};

template<>
struct functions_implementation<float>
{
    static inline float sqrt(float v)
    {
        return std::sqrt(v);
    }

    static inline float abs(float v)
    {
        return std::abs(v);
    }

    static constexpr inline float epsilon()
    {
        return std::numeric_limits<float>::epsilon();
    }
};

template<>
struct functions_implementation<double>
{
    static inline double sqrt(double v)
    {
        return std::sqrt(v);
    }

    static inline double abs(double v)
    {
        return std::abs(v);
    }

    static constexpr inline double epsilon()
    {
        return std::numeric_limits<double>::epsilon();
    }
};

template<>
struct functions_implementation<long double>
{
    static inline long double sqrt(long double v)
    {
        return std::sqrt(v);
    }

    static inline long double abs(long double v)
    {
        return std::abs(v);
    }

    static constexpr inline long double epsilon()
    {
        return std::numeric_limits<long double>::epsilon();
    }
};

//flag used for indicating if equality comparisons should be performed accurately (i.e abs(a-b)<=epsilon)
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

//type declaration
template<class T, size_t D>
class vector;

template<class T, size_t N, size_t M>
class matrix;

enum class equation_system_type
{
    determinate,
    indeterminate,
    contradictory
};

template<class T, size_t M>
class equation_system_solution;

NAMESPACE_LINEAR_ALGEBRA_END