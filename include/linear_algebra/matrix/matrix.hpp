#pragma once

#include "../linear_algebra_common_functions.hpp"

NAMESPACE_LINEAR_ALGEBRA_BEGIN

NAMESPACE_LINEAR_ALGEBRA_IO_BEGIN

template<class TO, size_t NO, size_t MO>
std::ostream& operator<<(std::ostream& os, const matrix<TO, NO, MO>& m);

NAMESPACE_LINEAR_ALGEBRA_IO_END

template<class... MS>
class matrix_multiplication_proxy
{
    //static_assert(std::conjunction_v<is_of_template_v<MS, matrix>...>, "Template arguments must be of matrix type");
    
    template<class... MS>
    friend class matrix_multiplication_proxy;
    template<class T, size_t N, size_t M>
    friend class matrix;
private:
    template<class... TS>
    struct matrix_multiplication_result_mathematical_field_type {};
    
    template<class T, class... TS>
    struct matrix_multiplication_result_mathematical_field_type<T, TS...> 
    {
        using type = inner_product_result_t<T, typename matrix_multiplication_result_mathematical_field_type<TS...>::type>;
    };

    template<class T1, class T2>
    struct matrix_multiplication_result_mathematical_field_type<T1, T2>
    {
        using type = inner_product_result_t<T1, T2>;
    };
public:
    static constexpr size_t result_rows = std::tuple_element_t<0, std::tuple<MS...>>::rows;
    static constexpr size_t result_columns = std::tuple_element_t<sizeof...(MS) - 1, std::tuple<MS...>>::columns;
    using result_mathematical_field_type = typename matrix_multiplication_result_mathematical_field_type<typename MS::mathematical_field_type...>::type;
private:
    std::tuple<const MS&...> _matrices;

    matrix_multiplication_proxy(std::tuple<const MS&...> matrices) :
        _matrices(matrices)
    {
    }
public:
    matrix_multiplication_proxy() = delete;
    template<class... MOS>
    matrix_multiplication_proxy(const matrix_multiplication_proxy<MOS...>& other) = delete;
    template<class... MOS>
    matrix_multiplication_proxy(matrix_multiplication_proxy<MOS...>&& other) = delete;

    template<class... MOS>
    matrix_multiplication_proxy<MS...>& operator=(const matrix_multiplication_proxy<MOS...>& other) = delete;
    template<class... MOS>
    matrix_multiplication_proxy<MS...>& operator=(matrix_multiplication_proxy<MOS...>&& other) = delete;
public:
    template<class... MSO, typename = typename std::enable_if_t<result_columns == matrix_multiplication_proxy<MSO...>::result_rows && can_calculate_inner_product_v<result_mathematical_field_type, typename matrix_multiplication_proxy<MSO...>::result_mathematical_field_type>>>
    matrix_multiplication_proxy<MS..., MSO...> operator*(const matrix_multiplication_proxy<MSO...>& other) const;
    template<class TO, size_t P, typename = typename std::enable_if_t<can_calculate_inner_product_v<TO, result_mathematical_field_type>>>
    matrix_multiplication_proxy<MS..., matrix<TO, matrix_multiplication_proxy<MS...>::result_columns, P>> operator*(const matrix<TO, matrix_multiplication_proxy<MS...>::result_columns, P>& other) const;

    template<class TO, typename = typename std::enable_if_t<can_calculate_inner_product_v<TO, result_mathematical_field_type>>>
    vector<inner_product_result_t<typename matrix_multiplication_proxy<MS...>::result_mathematical_field_type, TO>, matrix_multiplication_proxy<MS...>::result_rows> operator*(const vector<TO, matrix_multiplication_proxy<MS...>::result_columns>& other) const;
public:
    matrix<typename matrix_multiplication_proxy<MS...>::result_mathematical_field_type, matrix_multiplication_proxy<MS...>::result_rows, matrix_multiplication_proxy<MS...>::result_columns> operator()() const;
    matrix<typename matrix_multiplication_proxy<MS...>::result_mathematical_field_type, matrix_multiplication_proxy<MS...>::result_rows, matrix_multiplication_proxy<MS...>::result_columns> operator*() const;
};

template<class T, size_t N, size_t M>
class matrix
{
    static_assert(N != 0 && M != 0, "Matrix dimensions must be at least 1!");
    static_assert(is_valid_mathematical_field_v<T>, "Matrix element type must satisfy valid_mathematical_field concept!");

    template<class TO, size_t NO, size_t MO>
    friend class matrix;
    template<class T, size_t D>
    friend class vector;
private:
    class matrix_storage_static
    {
    private:
        T _mat[N][M];
    public:
        inline T* operator[](size_t row);
        inline const T* operator[](size_t row) const;
        
        inline T* data();
        inline const T* data() const;
    };

    class matrix_storage_dynamic
    {
    private:
        T* _mat = nullptr;
    public:
        inline matrix_storage_dynamic();
        inline matrix_storage_dynamic(matrix<T,N,M>::matrix_storage_dynamic&& other);
        inline matrix_storage_dynamic& operator=(matrix_storage_dynamic&& other);
        inline ~matrix_storage_dynamic();
    public:
        inline T* operator[](size_t row);
        inline const T* operator[](size_t row) const;
        
        inline T* data();
        inline const T* data() const;
    };

    static constexpr bool is_big_matrix = N * M * sizeof(T) >= static_storage_max_size;

    using storage_type = typename std::conditional_t<is_big_matrix, matrix_storage_dynamic, matrix_storage_static>;

    storage_type _mat;
public:
    using mathematical_field_type = T;

    static constexpr size_t rows = N;
    static constexpr size_t columns = M;
public:
    //default constructor

    matrix();

    //copy, move constructors and operators

    //same type, same dimensions

    matrix(const matrix<T, N, M>& other);

    matrix(matrix<T, N, M>&& other);

    matrix<T, N, M>& operator=(const matrix<T, N, M>& other);

    matrix<T, N, M>& operator=(matrix<T, N, M>&& other);

    //different type, same dimension

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(const matrix<TO, N, M>& other);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(matrix<TO, N, M>&& other);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(const matrix<TO, N, M>& other);
    
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(matrix<TO, N, M>&& other);

    //different type, different dimensions

    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(const matrix<TO, NO, MO>& other);

    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(matrix<TO, NO, MO>&& other);

    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(const matrix<TO, NO, MO>& other);

    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(matrix<TO, NO, MO>&& other);

    //regular constructors

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(const TO& v);

    template<class... TOS, typename = typename std::enable_if_t<(sizeof...(TOS) > 1) && (std::is_convertible_v<TOS, T> && ...)>>
    matrix(const TOS&... vs);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(TO&& v);
    
    template<class... TOS, typename = typename std::enable_if_t<(sizeof...(TOS) > 1) && (std::is_convertible_v<TOS, T> && ...)>>
    matrix(TOS&&... vs);

    matrix(std::initializer_list<T> vs);
    
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(std::initializer_list<TO> vs);

    matrix(std::initializer_list<std::initializer_list<T>> vs);
    
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(std::initializer_list<std::initializer_list<TO>> vs);

    //regular operators

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(const TO& v);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(TO&& v);
    
    matrix<T, N, M>& operator=(std::initializer_list<T> vs);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(std::initializer_list<TO> vs);

    matrix<T, N, M>& operator=(std::initializer_list<std::initializer_list<T>> vs);
    
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(std::initializer_list<std::initializer_list<TO>> vs);

    //from proxy
    template<class... MOS, typename = typename std::enable_if_t<std::is_convertible_v<typename matrix_multiplication_proxy<MOS...>::result_mathematical_field_type, T>>>
    matrix(const matrix_multiplication_proxy<MOS...>& proxy);
    template<class... MOS, typename = typename std::enable_if_t<std::is_convertible_v<typename matrix_multiplication_proxy<MOS...>::result_mathematical_field_type, T>>>
    matrix(matrix_multiplication_proxy<MOS...>&& proxy);

    template<class... MOS, typename = typename std::enable_if_t<std::is_convertible_v<typename matrix_multiplication_proxy<MOS...>::result_mathematical_field_type, T>>>
    matrix<T, N, M>& operator=(const matrix_multiplication_proxy<MOS...>& proxy);
    template<class... MOS, typename = typename std::enable_if_t<std::is_convertible_v<typename matrix_multiplication_proxy<MOS...>::result_mathematical_field_type, T>>>
    matrix<T, N, M>& operator=(matrix_multiplication_proxy<MOS...>&& proxy);
public:
    //matrix-matrix operators

    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<can_be_added_v<T, TO>>>
    matrix<addition_result_t<T, TO>, smaller<N, NO>, smaller<M, MO>> operator+(const matrix<TO, NO, MO>& other) const;
    
    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<can_be_subtracted_v<T, TO>>>
    matrix<subtraction_result_t<T, TO>, smaller<N, NO>, smaller<M, MO>> operator-(const matrix<TO, NO, MO>& other) const;
    
    template<class TO, size_t P, typename = typename std::enable_if_t<can_calculate_inner_product_v<T, TO>>>
    matrix_multiplication_proxy<matrix<T, N, M>, matrix<TO, M, P>> operator*(const matrix<TO, M, P>& other) const;
    
    template<class... MOS, typename = typename std::enable_if_t<matrix_multiplication_proxy<MOS...>::result_rows == N && matrix_multiplication_proxy<MOS...>::result_rows == N>>
    matrix_multiplication_proxy<matrix<T, N, M>, MOS...> operator*(const matrix_multiplication_proxy<MOS...>& other) const;

    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator+=(const matrix<TO, NO, MO>& other);
    
    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator-=(const matrix<TO, NO, MO>& other);
    
    template<class TO, size_t P, typename = typename std::enable_if_t<M == P && can_calculate_inner_product_v<T, TO> && std::is_convertible_v<inner_product_result_t<T, TO>, T>>>
    matrix<T, N, M>& operator*=(const matrix<TO, M, P>& other);

    //matrix-scalar operators

    matrix<T, N, M> operator-() const;

    template<class TO, typename = typename std::enable_if_t<can_be_multiplied_v<T, TO> && std::is_convertible_v<TO, T>>>
    matrix<multiplication_result_t<T, TO>, N, M> operator*(const TO& v) const;
    
    template<class TO, typename = typename std::enable_if_t<can_be_divided_v<T, TO> && std::is_convertible_v<TO, T>>>
    matrix<division_result_t<T, TO>, N, M> operator/(const TO& v) const;

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator*=(const TO& v);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator/=(const TO& v);

    //matrix-vector operators

    template<class TO, typename = typename std::enable_if_t<can_calculate_inner_product_v<T, TO>>>
    vector<inner_product_result_t<T, TO>, N> operator*(const vector<TO, M>& vec) const;
public:
    //matrix info and accessors
    T* operator[](size_t x);
    const T* operator[](size_t x) const;

    //matrix data is ordered row by row (i.e row|row|row|...|row)
    //same rule applies for iterators

    T* data();
    const T* data() const;

    T* begin();
    const T* begin() const;

    T* end();
    const T* end() const;
public:
    //comparison operators

    //if two matrices with identical dimensions are compared, they are equal only if every corresponding element is equal
    //and inequal if there is one element that is not equal to corresponding one
    //else matrices are always inequal

    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<use_high_quality_equality_comparison ? high_quality_equality_comparable<T1, T2> || equality_comparable_v<T1, T2 > : equality_comparable_v<T1, T2>>>
    bool operator==(const matrix<TO, NO, MO>& other) const;
    
    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<use_high_quality_inequality_comparison ? high_quality_inequality_comparable<T1, T2> || inequality_comparable_v<T1, T2 > : inequality_comparable_v<T1, T2>>>
    bool operator!=(const matrix<TO, NO, MO>& other) const;
private:
    //helpers

    template<class TO, size_t NO, size_t MO>
    static inline void swap_rows(matrix<TO, NO, MO>& matrix, size_t r1, size_t r2, size_t start_column = 0, size_t end_column = MO);

    template<class TO, size_t NO, size_t MO>
    static inline void swap_columns(matrix<TO, NO, MO>& matrix, size_t c1, size_t c2, size_t start_row = 0, size_t end_row = NO);
public:
    //matrix operations

    template<typename = typename std::enable_if_t<N == M>>
    T determinant() const;

    template<typename = typename std::enable_if_t<N == M>>
    std::optional<matrix<T, N, M>> inverted() const;

    size_t rank() const;

    matrix<T, M, N> transposed() const;
    
    template<typename = typename std::enable_if_t<N == M>>
    matrix<T, M, N>& transpose();
public:
    template<typename = typename std::enable_if_t<N == M>>
    static const matrix<T, N, M>& MULTIPLICATIVE_IDENTITY();

    static const matrix<T, N, M>& ADDITIVE_IDENTITY();
public:
    template<class TO, size_t NO, size_t MO>
    friend std::ostream& LINEAR_ALGEBRA_IO::operator<<(std::ostream& os, const matrix<TO, NO, MO>& m);

    template<class T, size_t N, size_t M>
    friend equation_system_solution<T, M> solve_equation_system(matrix<T, N, M>&& coefficents, vector<T, N>&& constant_terms);

    template<class T, size_t N, size_t M>
    friend equation_system_solution<T, M> solve_equation_system(const matrix<T, N, M>& coefficents, const vector<T, N>& constant_terms);
};

NAMESPACE_LINEAR_ALGEBRA_END