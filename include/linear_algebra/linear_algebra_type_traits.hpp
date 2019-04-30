#pragma once

#include "linear_algebra_common.hpp"

NAMESPACE_LINEAR_ALGEBRA_BEGIN

template <class T1, class T2>
class can_be_added
{
private:
    template<class TO1, class TO2>
    static std::true_type test(decltype(std::declval<TO1>() + std::declval<TO2>())*);
    template<class TO1, class TO2>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

template <class T1, class T2>
class can_be_subtracted
{
private:
    template<class TO1, class TO2>
    static std::true_type test(decltype(std::declval<TO1>() - std::declval<TO2>())*);
    template<class TO1, class TO2>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

template <class T1, class T2>
class can_be_multiplied
{
private:
    template<class TO1, class TO2>
    static std::true_type test(decltype(std::declval<TO1>() * std::declval<TO2>())*);
    template<class TO1, class TO2>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

template <class T1, class T2>
class can_be_divided
{
private:
    template<class TO1, class TO2>
    static std::true_type test(decltype(std::declval<TO1>() / std::declval<TO2>())*);
    template<class TO1, class TO2>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

template <class T1, class T2>
class can_calculate_inner_product
{
private:
    template<class TO1, class TO2>
    static std::true_type test(decltype(std::declval<TO1>() * std::declval<TO2>() + std::declval<TO1>() * std::declval<TO2>())*);
    template<class TO1, class TO2>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

template<class T1, class T2>
constexpr bool can_be_added_v = can_be_added<T1, T2>::value;

template<class T1, class T2>
constexpr bool can_be_subtracted_v = can_be_subtracted<T1, T2>::value;

template<class T1, class T2>
constexpr bool can_be_multiplied_v = can_be_multiplied<T1, T2>::value;

template<class T1, class T2>
constexpr bool can_be_divided_v = can_be_divided<T1, T2>::value;

template<class T1, class T2>
constexpr bool can_calculate_inner_product_v = can_calculate_inner_product<T1, T2>::value;

template <class T1, class T2>
class addition_result
{
private:
    template<class TO1, class TO2>
    static decltype(std::declval<TO1>() + std::declval<TO2>()) test(decltype(std::declval<TO1>() + std::declval<TO2>())*);
    template<class TO1, class TO2>
    static void test(...);
public:
    using type = decltype(test<T1, T2>(nullptr));
};

template <class T1, class T2>
class subtraction_result
{
private:
    template<class TO1, class TO2>
    static decltype(std::declval<TO1>() - std::declval<TO2>()) test(decltype(std::declval<TO1>() - std::declval<TO2>())*);
    template<class TO1, class TO2>
    static void test(...);
public:
    using type = decltype(test<T1, T2>(nullptr));
};

template <class T1, class T2>
class multiplication_result
{
private:
    template<class TO1, class TO2>
    static decltype(std::declval<TO1>() * std::declval<TO2>()) test(decltype(std::declval<TO1>() * std::declval<TO2>())*);
    template<class TO1, class TO2>
    static void test(...);
public:
    using type = decltype(test<T1, T2>(nullptr));
};

template <class T1, class T2>
class division_result
{
private:
    template<class TO1, class TO2>
    static decltype(std::declval<TO1>() / std::declval<TO2>()) test(decltype(std::declval<TO1>() / std::declval<TO2>())*);
    template<class TO1, class TO2>
    static void test(...);
public:
    using type = decltype(test<T1, T2>(nullptr));
};

template <class T1, class T2>
class inner_product_result
{
private:
    template<class TO1, class TO2>
    static decltype(std::declval<TO1>() * std::declval<TO2>() + std::declval<TO1>() * std::declval<TO2>()) test(decltype(std::declval<T1>() * std::declval<T2>() + std::declval<T1>() * std::declval<T2>())*);
    template<class TO1, class TO2>
    static void test(...);
public:
    using type = decltype(test<T1, T2>(nullptr));
};

template<class T1, class T2>
using addition_result_t = typename addition_result<T1, T2>::type;

template<class T1, class T2>
using subtraction_result_t = typename subtraction_result<T1, T2>::type;

template<class T1, class T2>
using multiplication_result_t = typename multiplication_result<T1, T2>::type;

template<class T1, class T2>
using division_result_t = typename division_result<T1, T2>::type;

template<class T1, class T2>
using inner_product_result_t = typename inner_product_result<T1, T2>::type;

template<class T>
class has_unary_minus
{
private:
    template<class TO>
    static std::true_type test(decltype(-std::declval<TO>())*);
    template<class TO>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T>(nullptr))::value;;
};

template<class T>
constexpr bool has_unary_minus_v = has_unary_minus<T>::value;

template <class T1, class T2>
class equality_comparable
{
private:
    template<class TO1, class TO2>
    static std::true_type test(decltype(std::declval<TO1>() == std::declval<TO2>())*);
    template<class TO1, class TO2>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

template<class T1, class T2>
constexpr bool equality_comparable_v = equality_comparable<T1, T2>::value;

template <class T1, class T2>
class inequality_comparable
{
private:
    template<class TO1, class TO2>
    static std::true_type test(decltype(std::declval<TO1>() != std::declval<TO2>())*);
    template<class TO1, class TO2>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

template<class T1, class T2>
constexpr bool inequality_comparable_v = inequality_comparable<T1, T2>::value;

template <class T1, class T2>
class less_comparable
{
private:
    template<class TO1, class TO2>
    static std::true_type test(decltype(std::declval<TO1>() < std::declval<TO2>())*);
    template<class TO1, class TO2>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

template<class T1, class T2>
constexpr bool less_comparable_v = less_comparable<T1, T2>::value;

template <class T1, class T2>
class greater_comparable
{
private:
    template<class TO1, class TO2>
    static std::true_type test(decltype(std::declval<TO1>() > std::declval<TO2>())*);
    template<class TO1, class TO2>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

template<class T1, class T2>
constexpr bool greater_comparable_v = greater_comparable<T1, T2>::value;

template <class T1, class T2>
class less_equal_comparable
{
private:
    template<class TO1, class TO2>
    static std::true_type test(decltype(std::declval<TO1>() <= std::declval<TO2>())*);
    template<class TO1, class TO2>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

template<class T1, class T2>
constexpr bool less_equal_comparable_v = less_equal_comparable<T1, T2>::value;

template <class T1, class T2>
class greater_equal_comparable
{
private:
    template<class TO1, class TO2>
    static std::true_type test(decltype(std::declval<TO1>() >= std::declval<TO2>())*);
    template<class TO1, class TO2>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

template<class T1, class T2>
constexpr bool greater_equal_comparable_v = greater_equal_comparable<T1, T2>::value;

template<size_t D1, size_t D2>
constexpr size_t bigger = D1 > D2 ? D1 : D2;

template<size_t D1, size_t D2>
constexpr size_t smaller = D1 < D2 ? D1 : D2;

#define ADDITIVE_IDENTITY additive_identity
#define MULTIPLICATIVE_IDENTITY multiplicative_identity

template<class T>
class has_additive_identity
{
private:
    template<class TO>
    static std::true_type test(decltype(TO::ADDITIVE_IDENTITY)*);
    template<class TO>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template<class T>
class has_zero
{
private:
    template<class TO>
    static std::true_type test(decltype(static_cast<TO>(0))*);
    template<class TO>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template<class T>
class has_multiplicative_identity
{
private:
    template<class TO>
    static std::true_type test(decltype(TO::MULTIPLICATIVE_IDENTITY)*);
    template<class TO>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template<class T>
class has_one
{
private:
    template<class TO>
    static std::true_type test(decltype(static_cast<TO>(1))*);
    template<class TO>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template<class T>
constexpr bool has_additive_identity_v = has_additive_identity<T>::value;

template<class T>
constexpr bool has_zero_v = has_zero<T>::value;

template<class T>
constexpr bool has_multiplicative_identity_v = has_multiplicative_identity<T>::value;

template<class T>
constexpr bool has_one_v = has_one<T>::value;

//this does not check all requirements of a mathematical field (only those that can be checked by computer)
template<class T>
constexpr bool is_valid_mathematical_field_v = 
    std::is_same_v<addition_result_t<T, T>, T> && 
    std::is_same_v<subtraction_result_t<T, T>, T> && 
    std::is_same_v<multiplication_result_t<T, T>, T> &&
    std::is_same_v<division_result_t<T, T>, T> &&
    std::is_same_v<inner_product_result_t<T, T>, T> &&
    ((has_additive_identity_v<T> && has_multiplicative_identity_v<T>) || (std::is_arithmetic_v<T> && has_zero_v<T> && has_one_v<T>)) &&
    std::is_default_constructible_v<T> && std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> && std::is_move_constructible_v<T> && std::is_move_assignable_v<T> &&
    has_unary_minus_v<T> && equality_comparable_v<T,T> && inequality_comparable_v<T,T>;

template<class T, typename = typename std::enable_if_t<is_valid_mathematical_field_v<T>>>
const T& get_additive_identity()
{
    if constexpr (has_additive_identity_v<T>)
    {
        return T::ADDITIVE_IDENTITY;
    }
    else
    {
        static T zero = static_cast<T>(0);
        return zero;
    }
}

template<class T, typename = typename std::enable_if_t<is_valid_mathematical_field_v<T>>>
const T& get_multiplicative_identity()
{
    if constexpr(has_multiplicative_identity_v<T>)
    {
        return T::MULTIPLICATIVE_IDENTITY;
    }
    else
    {
        static T one = static_cast<T>(1);
        return one;
    }
}

NAMESPACE_LINEAR_ALGEBRA_END