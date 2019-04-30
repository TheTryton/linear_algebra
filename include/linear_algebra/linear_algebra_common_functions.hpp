#pragma once

#include "linear_algebra_type_traits.hpp"

NAMESPACE_LINEAR_ALGEBRA_BEGIN

//specializable struct used for overloading commonly used functions for non-standard types
template<class T>
struct functions_implementation
{
    static void sqrt(T v);

    static void abs(T v);

    static constexpr void epsilon();
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

template<class T>
constexpr bool has_sqrt_implementation_v = std::is_same_v<decltype(functions_implementation<T>::sqrt(std::declval<T>())), T>;

template<class T>
constexpr bool has_abs_implementation_v = std::is_same_v<decltype(functions_implementation<T>::sqrt(std::declval<T>())), T>;

template<class T>
constexpr bool has_epsilon_implementation_v = std::is_same_v<decltype(functions_implementation<T>::epsilon()), T>;

//flag used for indicating if equality comparisons should be performed accurately (i.e abs(a-b)<=epsilon)
constexpr bool use_high_quality_equality_comparison = false;

template<class T1, class T2>
constexpr bool high_quality_equality_comparable_v = can_be_subtracted_v<T1, T2> && has_epsilon_implementation_v<subtraction_result_t<T1, T2>> && has_abs_implementation_v<subtraction_result_t<T1, T2>> && less_equal_comparable_v<subtraction_result_t<T1, T2>, subtraction_result_t<T1, T2>>;

template<class T1, class T2>
constexpr bool high_quality_inequality_comparable_v = can_be_subtracted_v<T1, T2> && has_epsilon_implementation_v<subtraction_result_t<T1, T2>> && has_abs_implementation_v<subtraction_result_t<T1, T2>> && greater_comparable_v<subtraction_result_t<T1, T2>, subtraction_result_t<T1, T2>>;

template<class T1, class T2, typename = typename std::enable_if_t<use_high_quality_equality_comparison ? high_quality_equality_comparable_v<T1,T2> || equality_comparable_v<T1, T2 > : equality_comparable_v<T1, T2>>>
inline bool equal(const T1& a, const T2& b)
{
    if constexpr (use_high_quality_equality_comparison && high_quality_equality_comparable_v<T1, T2>)
    {
        return functions_implementation<subtraction_result_t<T1, T2>>::abs(a - b) <= functions_implementation<subtraction_result_t<T1, T2>>::epsilon();
    }
    else
    {
        return a == b;
    }
}

template<class T1, class T2, typename = typename std::enable_if_t<use_high_quality_equality_comparison ? high_quality_inequality_comparable_v<T1, T2> || inequality_comparable_v<T1, T2 > : inequality_comparable_v<T1, T2>>>
inline bool inequal(const T1& a, const T2& b)
{
    if constexpr (use_high_quality_equality_comparison && high_quality_inequality_comparable_v<T1, T2>)
    {
        return functions_implementation<subtraction_result_t<T1, T2>>::abs(a - b) > functions_implementation<subtraction_result_t<T1, T2>>::epsilon();
    }
    else
    {
        return a != b;
    }
}

NAMESPACE_LINEAR_ALGEBRA_END