#pragma once

#include "matrix.hpp"
#include "../vector/vector.hpp"
#include "../vector/vector.inl"

NAMESPACE_LINEAR_ALGEBRA_BEGIN

//matrix multiplication proxy

namespace detail
{
    template<class T, class TO, size_t N, size_t M, size_t P>
    matrix<inner_product_result_t<T, TO>, N, P> multiply_matrices(const matrix<T, N, M>& m1, const matrix<TO, M, P>& m2)
    {
        matrix<inner_product_result_t<T, TO>, N, P> result;

#if USE_OPENMP
        if (matrix<T, N, M>::is_big_matrix || matrix<TO, M, P>::is_big_matrix)
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N); row++)
            {
                for (size_t column = 0; column < P; column++)
                {
                    //calculating column-row inner product

                    auto inner_product = get_additive_identity<inner_product_result_t<T, TO>>();

                    for (size_t k = 0; k < M; k++)
                    {
                        inner_product += m1[row][k] * m2[k][column];
                    }

                    result[row][column] = inner_product;
                }
            }
        }
        else
        {
            for (size_t row = 0; row < N; row++)
            {
                for (size_t column = 0; column < P; column++)
                {
                    //calculating column-row inner product

                    auto inner_product = get_additive_identity<inner_product_result_t<T, TO>>();

                    for (size_t k = 0; k < M; k++)
                    {
                        inner_product += m1[row][k] * m2[k][column];
                    }

                    result[row][column] = inner_product;
                }
            }
        }
#else
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < P; column++)
            {
                //calculating column-row inner product

                auto inner_product = additive_identity<inner_product_result_t<T, TO>>();

                for (size_t k = 0; k < M; k++)
                {
                    inner_product += m1[row][k] * m2[k][column];
                }

                result._mat[row][column] = inner_product;
            }
        }
#endif

        return result;
    }

    struct matrix_size
    {
        size_t rows;
        size_t columns;
    };

    template<size_t S>
    constexpr std::array<std::array<size_t, S>, S> get_multiplication_bracketing(const std::array<matrix_size, S>& sizes)
    {
        std::array<std::array<size_t, S >, S > m = {};
        for (size_t i = 0; i < S; i++)
        {
            m[i][i] = 0;
        }
        std::array<std::array<size_t, S >, S > s = {};

        for (size_t l = 2; l <= S; l++)
        {
            for (size_t b = 0; b < S - l + 1; b++)
            {
                size_t e = b + l - 1;

                m[b][e] = std::numeric_limits<size_t>::max();

                for (size_t split = b; split < e; split++)
                {
                    size_t cost = m[b][split] + m[split + 1][e] + sizes[b].rows * sizes[split].columns * sizes[e].columns;
                    if (cost < m[b][e])
                    {
                        m[b][e] = cost;
                        s[b][e] = split + 1;
                    }
                }
            }
        }

        for (size_t i = 0; i < S; i++)
        {
            s[i][i] = i;
        }

        return s;
    }

    template<size_t b, size_t e, class... TS, size_t... NS, size_t... MS>
    auto multiply_in_bracketed_order(const std::tuple<const matrix<TS, NS, MS>&...>& matrices)
    {
        constexpr size_t S = sizeof...(TS);
        constexpr auto arr = std::array<matrix_size, sizeof...(TS)>({ matrix_size{ matrix<TS, NS, MS>::rows,  matrix<TS, NS, MS>::columns}... });
        constexpr auto s = get_multiplication_bracketing(arr);

        if constexpr (b == e)
        {
            return std::get<s[b][e]>(matrices);
        }
        if constexpr (s[b][e] > b && s[b][e] <= e)
        {
            return multiply_matrices(multiply_in_bracketed_order<b, s[b][e] - 1>(matrices), multiply_in_bracketed_order<s[b][e], e>(matrices));
        }
    }
}

template<class... MS>
template<class... MSO, typename>
matrix_multiplication_proxy<MS..., MSO...> matrix_multiplication_proxy<MS...>::operator*(const matrix_multiplication_proxy<MSO...>& other) const
{
    return matrix_multiplication_proxy<MS..., MSO...>(std::tuple_cat(_matrices, other._matrices));
}

template<class... MS>
template<class TO, size_t P, typename>
matrix_multiplication_proxy<MS..., matrix<TO, matrix_multiplication_proxy<MS...>::result_columns, P>> matrix_multiplication_proxy<MS...>::operator*(const matrix<TO, matrix_multiplication_proxy<MS...>::result_columns, P>& other) const
{
    return matrix_multiplication_proxy<MS..., matrix<TO, result_columns, P>>(std::tuple_cat(_matrices, std::tuple<const matrix<TO, result_columns, P>&>(other)));
}

template<class... MS>
template<class TO, typename>
vector<inner_product_result_t<typename matrix_multiplication_proxy<MS...>::result_mathematical_field_type, TO>, matrix_multiplication_proxy<MS...>::result_rows> matrix_multiplication_proxy<MS...>::operator*(const vector<TO, matrix_multiplication_proxy<MS...>::result_columns>& other) const
{
    matrix<TO, matrix_multiplication_proxy<MS...>::result_columns, 1> vector_copy;

    for (size_t d = 0; d < matrix_multiplication_proxy<MS...>::result_columns; d++)
    {
        vector_copy[d][0] = other[d];
    }

    auto result_matrix = *matrix_multiplication_proxy<MS..., matrix<TO, matrix_multiplication_proxy<MS...>::result_columns, 1>>(std::tuple_cat(_matrices, std::tuple<const matrix<TO, matrix_multiplication_proxy<MS...>::result_columns, 1>&>(vector_copy)));

    vector<inner_product_result_t<typename matrix_multiplication_proxy<MS...>::result_mathematical_field_type, TO>, matrix_multiplication_proxy<MS...>::result_rows> result_vector;

    for (size_t d = 0; d < matrix_multiplication_proxy<MS...>::result_rows; d++)
    {
        result_vector[d] = result_matrix[d][0];
    }

    return result_vector;
}

template<class... MS>
matrix<typename matrix_multiplication_proxy<MS...>::result_mathematical_field_type, matrix_multiplication_proxy<MS...>::result_rows, matrix_multiplication_proxy<MS...>::result_columns> matrix_multiplication_proxy<MS...>::operator()() const
{
    return detail::multiply_in_bracketed_order<0, sizeof...(MS) - 1>(_matrices);
}

template<class... MS>
matrix<typename matrix_multiplication_proxy<MS...>::result_mathematical_field_type, matrix_multiplication_proxy<MS...>::result_rows, matrix_multiplication_proxy<MS...>::result_columns> matrix_multiplication_proxy<MS...>::operator*() const
{
    return detail::multiply_in_bracketed_order<0, sizeof...(MS) - 1>(_matrices);
}

//static storage

template<class T, size_t N, size_t M>
inline T* matrix<T, N, M>::matrix_storage_static::operator[](size_t row)
{
    return _mat[row];
}

template<class T, size_t N, size_t M>
inline const T* matrix<T, N, M>::matrix_storage_static::operator[](size_t row) const
{
    return _mat[row];
}

template<class T, size_t N, size_t M>
inline T* matrix<T, N, M>::matrix_storage_static::data()
{
    return _mat;
}

template<class T, size_t N, size_t M>
inline const T* matrix<T, N, M>::matrix_storage_static::data() const
{
    return _mat;
}

//dynamic storage

template<class T, size_t N, size_t M>
inline matrix<T, N, M>::matrix_storage_dynamic::matrix_storage_dynamic()
{
    _mat = new T[N*M];
}

template<class T, size_t N, size_t M>
inline matrix<T, N, M>::matrix_storage_dynamic::matrix_storage_dynamic(typename matrix<T, N, M>::matrix_storage_dynamic&& other) noexcept
{
    _mat = other._mat;
    other._mat = nullptr;
}

template<class T, size_t N, size_t M>
inline typename matrix<T, N, M>::matrix_storage_dynamic& matrix<T, N, M>::matrix_storage_dynamic::operator=(typename matrix<T, N, M>::matrix_storage_dynamic&& other) noexcept
{
    _mat = other._mat;
    other._mat = nullptr;
    return *this;
}

template<class T, size_t N, size_t M>
inline matrix<T, N, M>::matrix_storage_dynamic::~matrix_storage_dynamic()
{
    if (_mat)
    {
        delete[] _mat;
    }
}

template<class T, size_t N, size_t M>
inline T* matrix<T, N, M>::matrix_storage_dynamic::operator[](size_t row)
{
    return _mat + row * M;
}

template<class T, size_t N, size_t M>
inline const T* matrix<T, N, M>::matrix_storage_dynamic::operator[](size_t row) const
{
    return _mat + row * M;
}

template<class T, size_t N, size_t M>
inline T* matrix<T, N, M>::matrix_storage_dynamic::data()
{
    return _mat;
}

template<class T, size_t N, size_t M>
inline const T* matrix<T, N, M>::matrix_storage_dynamic::data() const
{
    return _mat;
}

//matrix

template<class T, size_t N, size_t M>
matrix<T, N, M>::matrix()
{
    //  filling matrix to initialize it with 0-oes
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] = get_additive_identity<T>();
        }
    }
#endif
}

template<class T, size_t N, size_t M>
matrix<T, N, M>::matrix(const matrix<T, N, M>& other)
{
    /*
        filling matrix NxM with matrix NxM
        matrix NxM elements are copied to
        elements in matrix MxN
    */
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] = static_cast<T>(other._mat[row][column]);
        }
    }
#endif
}

template<class T, size_t N, size_t M>
matrix<T, N, M>::matrix(matrix<T, N, M>&& other)
{
    /*
        filling matrix NxM with matrix NxM
        matrix NxM elements are copied to
        elements in matrix MxN

        move optimization for dynamic storage
    */
#if USE_OPENMP
    if (is_big_matrix)
    {
        if (std::is_same_v<storage_type, matrix_storage_dynamic>)
        {
            _mat = std::move(other._mat);
        }
        else
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N); row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    _mat[row][column] = static_cast<T>(other._mat[row][column]);
                }
            }
        }
    }
    else
    {
        if constexpr (std::is_same_v<storage_type, matrix_storage_dynamic>)
        {
            _mat = std::move(other._mat);
        }
        else
        {
            for (size_t row = 0; row < N; row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    _mat[row][column] = static_cast<T>(other._mat[row][column]);
                }
            }
        }
    }
#else
    if constexpr (std::is_same_v<storage_type, matrix_storage_dynamic>)
    {
        _mat = std::move(other._mat);
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
#endif
}

template<class T, size_t N, size_t M>
matrix<T, N, M>& matrix<T, N, M>::operator=(const matrix<T, N, M>& other)
{
    /*
        filling matrix NxM with matrix NxM
        matrix NxM elements are copied to
        elements in matrix MxN
    */
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] = static_cast<T>(other._mat[row][column]);
        }
    }
#endif
    return *this;
}

template<class T, size_t N, size_t M>
matrix<T, N, M>& matrix<T, N, M>::operator=(matrix<T, N, M>&& other)
{
    /*
        filling matrix NxM with matrix NxM
        matrix NxM elements are copied to
        elements in matrix MxN

        move optimization for dynamic storage
    */
#if USE_OPENMP
    if (is_big_matrix)
    {
        if (std::is_same_v<storage_type, matrix_storage_dynamic>)
        {
            _mat = std::move(other._mat);
        }
        else
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N); row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    _mat[row][column] = static_cast<T>(other._mat[row][column]);
                }
            }
        }
    }
    else
    {
        if constexpr (std::is_same_v<storage_type, matrix_storage_dynamic>)
        {
            _mat = std::move(other._mat);
        }
        else
        {
            for (size_t row = 0; row < N; row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    _mat[row][column] = static_cast<T>(other._mat[row][column]);
                }
            }
        }
    }
#else
    if constexpr (std::is_same_v<storage_type, matrix_storage_dynamic>)
    {
        _mat = std::move(other._mat);
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
#endif
    return *this;
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>::matrix(const matrix<TO, N, M>& other)
{
    /*
        filling matrix NxM with matrix NxM
        matrix NxM elements are copied to
        elements in matrix MxN
    */
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] = static_cast<T>(other._mat[row][column]);
        }
    }
#endif
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>::matrix(matrix<TO, N, M>&& other)
{
    /*
        filling matrix NxM with matrix NxM
        matrix NxM elements are copied to
        elements in matrix MxN
    */
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] = static_cast<T>(other._mat[row][column]);
        }
    }
#endif
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>& matrix<T, N, M>::operator=(const matrix<TO, N, M>& other)
{
    /*
        filling matrix NxM with matrix NxM
        matrix NxM elements are copied to
        elements in matrix MxN
    */
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] = static_cast<T>(other._mat[row][column]);
        }
    }
#endif
    return *this;
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>& matrix<T, N, M>::operator=(matrix<TO, N, M>&& other)
{
    /*
        filling matrix NxM with matrix NxM
        matrix NxM elements are copied to
        elements in matrix MxN
    */
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] = static_cast<T>(other._mat[row][column]);
        }
    }
#endif
    return *this;
}

template<class T, size_t N, size_t M>
template<class TO, size_t NO, size_t MO, typename>
matrix<T, N, M>::matrix(const matrix<TO, NO, MO>& other)
{
#if USE_OPENMP
    if (is_big_matrix)
    {
        /*
            filling matrix NxM with matrix NOxMO
            matrix NOxMO elements are copied to existing
            elements in matrix MxN (element 0,0 is copied to element 0,0)
            without getting out of matrix bounds
        */

#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(smaller<N, NO>); row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }

        /*
            case in which other matrix is smaller than this matrix
            we copy all of the other matrix elements
            in according places leaveing us with 3 submatrices
            which are uninitialized
        */

        //  filling left-bottom submatrix to initialize it with 0-oes
#pragma omp parallel for
        for (int row = static_cast<int>(smaller<N, NO>); row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }

        //  filling right-top submatrix to initialize it with 0-oes
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(smaller<N, NO>); row++)
        {
            for (size_t column = smaller<M, MO>; column < M; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }

        //  filling bottom-right submatrix to initialize it with 0-oes
#pragma omp parallel for
        for (int row = static_cast<int>(smaller<N, NO>); row < static_cast<int>(N); row++)
        {
            for (size_t column = smaller<M, MO>; column < M; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }
    }
    else
    {
        /*
        filling matrix NxM with matrix NOxMO
        matrix NOxMO elements are copied to existing
        elements in matrix MxN (element 0,0 is copied to element 0,0)
        without getting out of matrix bounds
    */

        for (size_t row = 0; row < smaller<N, NO>; row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }

        /*
            case in which other matrix is smaller than this matrix
            we copy all of the other matrix elements
            in according places leaveing us with 3 submatrices
            which are uninitialized
        */

        //  filling left-bottom submatrix to initialize it with 0-oes
        for (size_t row = smaller<N, NO>; row < N; row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }

        //  filling right-top submatrix to initialize it with 0-oes
        for (size_t row = 0; row < smaller<N, NO>; row++)
        {
            for (size_t column = smaller<M, MO>; column < M; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }

        //  filling bottom-right submatrix to initialize it with 0-oes
        for (size_t row = smaller<N, NO>; row < N; row++)
        {
            for (size_t column = smaller<M, MO>; column < M; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }
    }
#else
    /*
        filling matrix NxM with matrix NOxMO
        matrix NOxMO elements are copied to existing
        elements in matrix MxN (element 0,0 is copied to element 0,0)
        without getting out of matrix bounds
    */

    for (size_t row = 0; row < smaller<N, NO>; row++)
    {
        for (size_t column = 0; column < smaller<M, MO>; column++)
        {
            _mat[row][column] = static_cast<T>(other._mat[row][column]);
        }
    }

    /*
        case in which other matrix is smaller than this matrix
        we copy all of the other matrix elements
        in according places leaveing us with 3 submatrices
        which are uninitialized
    */

    //  filling left-bottom submatrix to initialize it with 0-oes
    for (size_t row = smaller<N, NO>; row < N; row++)
    {
        for (size_t column = 0; column < smaller<M, MO>; column++)
        {
            _mat[row][column] = get_additive_identity<T>();
        }
    }

    //  filling right-top submatrix to initialize it with 0-oes
    for (size_t row = 0; row < smaller<N, NO>; row++)
    {
        for (size_t column = smaller<M, MO>; column < M; column++)
        {
            _mat[row][column] = get_additive_identity<T>();
        }
    }

    //  filling bottom-right submatrix to initialize it with 0-oes
    for (size_t row = smaller<N, NO>; row < N; row++)
    {
        for (size_t column = smaller<M, MO>; column < M; column++)
        {
            _mat[row][column] = get_additive_identity<T>();
        }
    }
#endif
}

template<class T, size_t N, size_t M>
template<class TO, size_t NO, size_t MO, typename>
matrix<T, N, M>::matrix(matrix<TO, NO, MO>&& other)
{
#if USE_OPENMP
    if (is_big_matrix)
    {
        /*
            filling matrix NxM with matrix NOxMO
            matrix NOxMO elements are copied to existing
            elements in matrix MxN (element 0,0 is copied to element 0,0)
            without getting out of matrix bounds
        */

#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(smaller<N, NO>); row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }

        /*
            case in which other matrix is smaller than this matrix
            we copy all of the other matrix elements
            in according places leaveing us with 3 submatrices
            which are uninitialized
        */

        //  filling left-bottom submatrix to initialize it with 0-oes
#pragma omp parallel for
        for (int row = static_cast<int>(smaller<N, NO>); row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }

        //  filling right-top submatrix to initialize it with 0-oes
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(smaller<N, NO>); row++)
        {
            for (size_t column = smaller<M, MO>; column < M; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }

        //  filling bottom-right submatrix to initialize it with 0-oes
#pragma omp parallel for
        for (int row = static_cast<int>(smaller<N, NO>); row < static_cast<int>(N); row++)
        {
            for (size_t column = smaller<M, MO>; column < M; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }
    }
    else
    {
        /*
        filling matrix NxM with matrix NOxMO
        matrix NOxMO elements are copied to existing
        elements in matrix MxN (element 0,0 is copied to element 0,0)
        without getting out of matrix bounds
    */

        for (size_t row = 0; row < smaller<N, NO>; row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }

        /*
            case in which other matrix is smaller than this matrix
            we copy all of the other matrix elements
            in according places leaveing us with 3 submatrices
            which are uninitialized
        */

        //  filling left-bottom submatrix to initialize it with 0-oes
        for (size_t row = smaller<N, NO>; row < N; row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }

        //  filling right-top submatrix to initialize it with 0-oes
        for (size_t row = 0; row < smaller<N, NO>; row++)
        {
            for (size_t column = smaller<M, MO>; column < M; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }

        //  filling bottom-right submatrix to initialize it with 0-oes
        for (size_t row = smaller<N, NO>; row < N; row++)
        {
            for (size_t column = smaller<M, MO>; column < M; column++)
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }
    }
#else
    /*
        filling matrix NxM with matrix NOxMO
        matrix NOxMO elements are copied to existing
        elements in matrix MxN (element 0,0 is copied to element 0,0)
        without getting out of matrix bounds
    */

    for (size_t row = 0; row < smaller<N, NO>; row++)
    {
        for (size_t column = 0; column < smaller<M, MO>; column++)
        {
            _mat[row][column] = static_cast<T>(other._mat[row][column]);
        }
    }

    /*
        case in which other matrix is smaller than this matrix
        we copy all of the other matrix elements
        in according places leaveing us with 3 submatrices
        which are uninitialized
    */

    //  filling left-bottom submatrix to initialize it with 0-oes
    for (size_t row = smaller<N, NO>; row < N; row++)
    {
        for (size_t column = 0; column < smaller<M, MO>; column++)
        {
            _mat[row][column] = get_additive_identity<T>();
        }
    }

    //  filling right-top submatrix to initialize it with 0-oes
    for (size_t row = 0; row < smaller<N, NO>; row++)
    {
        for (size_t column = smaller<M, MO>; column < M; column++)
        {
            _mat[row][column] = get_additive_identity<T>();
        }
    }

    //  filling bottom-right submatrix to initialize it with 0-oes
    for (size_t row = smaller<N, NO>; row < N; row++)
    {
        for (size_t column = smaller<M, MO>; column < M; column++)
        {
            _mat[row][column] = get_additive_identity<T>();
        }
    }
#endif
}

template<class T, size_t N, size_t M>
template<class TO, size_t NO, size_t MO, typename>
matrix<T, N, M>& matrix<T, N, M>::operator=(const matrix<TO, NO, MO>& other)
{
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(smaller<N, NO>); row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < smaller<N, NO>; row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
#else
    for (size_t row = 0; row < smaller<N, NO>; row++)
    {
        for (size_t column = 0; column < smaller<M, MO>; column++)
        {
            _mat[row][column] = static_cast<T>(other._mat[row][column]);
        }
    }
#endif
    return *this;
}

template<class T, size_t N, size_t M>
template<class TO, size_t NO, size_t MO, typename>
matrix<T, N, M>& matrix<T, N, M>::operator=(matrix<TO, NO, MO>&& other)
{
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(smaller<N, NO>); row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < smaller<N, NO>; row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
    }
#else
    for (size_t row = 0; row < smaller<N, NO>; row++)
    {
        for (size_t column = 0; column < smaller<M, MO>; column++)
        {
            _mat[row][column] = static_cast<T>(other._mat[row][column]);
        }
    }
#endif
    return *this;
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>::matrix(const TO& v)
{
    //initializes all matrix elements with value v
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(v);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(v);
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] = static_cast<T>(v);
        }
    }
#endif
}

template<class T, size_t N, size_t M>
template<class... TOS, typename>
matrix<T, N, M>::matrix(const TOS&... vs)

{
    std::array<T, sizeof...(TOS)> elements = { static_cast<T>(vs)... };

    constexpr size_t DM = sizeof...(TOS) > N*M ? N * M : sizeof...(TOS);

    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            if (row*column < sizeof...(TOS))
            {
                _mat[row][column] = elements[row*M + column];
            }
            else
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }
    }
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>::matrix(TO&& v)
{
    //initializes all matrix elements with value v
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(v);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(v);
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] = static_cast<T>(v);
        }
    }
#endif
}

template<class T, size_t N, size_t M>
template<class... TOS, typename>
matrix<T, N, M>::matrix(TOS&&... vs)
{
    std::array<T, sizeof...(TOS)> elements = { static_cast<T>(vs)... };

    constexpr size_t DM = sizeof...(TOS) > N*M ? N * M : sizeof...(TOS);

    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            if (row*column < sizeof...(TOS))
            {
                _mat[row][column] = std::move(elements[row*M + column]);
            }
            else
            {
                _mat[row][column] = get_additive_identity<T>();
            }
        }
    }
}

template<class T, size_t N, size_t M>
matrix<T, N, M>::matrix(std::initializer_list<T> vs)
{
    //fills matrix with values from initializer_list row by row

    auto values = vs.begin();

    size_t row = 0;
    size_t column = 0;
    for (; row < N; row++)
    {
        for (column = 0; column < M; column++, values++)
        {
            if (values == vs.end())
            {
                goto initializer_list_end;
            }
            else
            {
                _mat[row][column] = *values;
            }
        }
    }

initializer_list_end:

    //every other element is set to 0

    for (; row < N; row++, column = 0)
    {
        for (; column < M; column++)
        {
            _mat[row][column] = get_additive_identity<T>();
        }
    }
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>::matrix(std::initializer_list<TO> vs)
{
    //fills matrix with values from initializer_list row by row

    auto values = vs.begin();

    size_t row = 0;
    size_t column = 0;
    for (; row < N; row++)
    {
        for (column = 0; column < M; column++, values++)
        {
            if (values == vs.end())
            {
                goto initializer_list_end;
            }
            else
            {
                _mat[row][column] = static_cast<T>(*values);
            }
        }
    }

initializer_list_end:

    //every other element is set to 0

    for (; row < N; row++, column = 0)
    {
        for (; column < M; column++)
        {
            _mat[row][column] = get_additive_identity<T>();
        }
    }
}

template<class T, size_t N, size_t M>
matrix<T, N, M>::matrix(std::initializer_list<std::initializer_list<T>> vs)
{
    //every subsequent initializer_list in initializer_list is treated as row of matrix elements values

    auto rows = vs.begin();
    size_t row = 0;

    for (; row < N; row++, rows++)
    {
        if (rows == vs.end())
        {
            break;
        }
        else
        {
            auto row_elements = (*rows).begin();
            auto row_elements_end = (*rows).end();

            size_t column = 0;

            for (; column < M; column++, row_elements++)
            {
                if (row_elements == row_elements_end)
                {
                    break;
                }
                else
                {
                    _mat[row][column] = *row_elements;
                }
            }

            //every other element in this row is set to 0
            if (row_elements == row_elements_end)
            {
                for (; column < M; column++)
                {
                    _mat[row][column] = get_additive_identity<T>();
                }
            }
        }
    }

    //every other row is set to 0

    for (; row < N; row++, rows++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] = get_additive_identity<T>();
        }
    }
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>::matrix(std::initializer_list<std::initializer_list<TO>> vs)
{
    //every subsequent initializer_list in initializer_list is treated as row of matrix elements values

    auto rows = vs.begin();
    size_t row = 0;

    for (; row < N; row++, rows++)
    {
        if (rows == vs.end())
        {
            break;
        }
        else
        {
            auto row_elements = (*rows).begin();
            auto row_elements_end = (*rows).end();

            size_t column = 0;

            for (; column < M; column++, row_elements++)
            {
                if (row_elements == row_elements_end)
                {
                    break;
                }
                else
                {
                    _mat[row][column] = static_cast<T>(*row_elements);
                }
            }

            //every other element in this row is set to 0
            if (row_elements == row_elements_end)
            {
                for (; column < M; column++)
                {
                    _mat[row][column] = get_additive_identity<T>();
                }
            }
        }
    }

    //every other row is set to 0

    for (; row < N; row++, rows++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] = get_additive_identity<T>();
        }
    }
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>& matrix<T, N, M>::operator=(const TO& v)
{
    //initializes all matrix elements with value v
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(v);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(v);
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] = static_cast<T>(v);
        }
    }
#endif
    return *this;
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>& matrix<T, N, M>::operator=(TO&& v)
{
    //initializes all matrix elements with value v
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(v);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(v);
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] = static_cast<T>(v);
        }
    }
#endif
    return *this;
}

template<class T, size_t N, size_t M>
matrix<T, N, M>& matrix<T, N, M>::operator=(std::initializer_list<T> vs)
{
    //fills matrix with values from initializer_list row by row

    auto values = vs.begin();

    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++, values++)
        {
            if (values == vs.end())
            {
                return *this;
            }
            else
            {
                _mat[row][column] = *values;
            }
        }
    }

    return *this;
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>& matrix<T, N, M>::operator=(std::initializer_list<TO> vs)
{
    //fills matrix with values from initializer_list row by row

    auto values = vs.begin();

    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++, values++)
        {
            if (values == vs.end())
            {
                return *this;
            }
            else
            {
                _mat[row][column] = static_cast<T>(*values);
            }
        }
    }

    return *this;
}

template<class T, size_t N, size_t M>
matrix<T, N, M>& matrix<T, N, M>::operator=(std::initializer_list<std::initializer_list<T>> vs)
{
    //every subsequent initializer_list in initializer_list is treated as row of matrix elements values

    auto rows = vs.begin();
    size_t row = 0;

    for (; row < N; row++, rows++)
    {
        if (rows == vs.end())
        {
            break;
        }
        else
        {
            auto row_elements = (*rows).begin();
            auto row_elements_end = (*rows).end();

            size_t column = 0;

            for (; column < M; column++, row_elements++)
            {
                if (row_elements == row_elements_end)
                {
                    break;
                }
                else
                {
                    _mat[row][column] = *row_elements;
                }
            }
        }
    }

    return *this;
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>& matrix<T, N, M>::operator=(std::initializer_list<std::initializer_list<TO>> vs)
{
    //every subsequent initializer_list in initializer_list is treated as row of matrix elements values

    auto rows = vs.begin();
    size_t row = 0;

    for (; row < N; row++, rows++)
    {
        if (rows == vs.end())
        {
            break;
        }
        else
        {
            auto row_elements = (*rows).begin();
            auto row_elements_end = (*rows).end();

            size_t column = 0;

            for (; column < M; column++, row_elements++)
            {
                if (row_elements == row_elements_end)
                {
                    break;
                }
                else
                {
                    _mat[row][column] = static_cast<T>(*row_elements);
                }
            }
        }
    }

    return *this;
}

template<class T, size_t N, size_t M>
template<class... MOS, typename>
matrix<T, N, M>::matrix(const matrix_multiplication_proxy<MOS...>& proxy) :
    matrix<T, N, M>(*proxy)
{
}

template<class T, size_t N, size_t M>
template<class... MOS, typename>
matrix<T, N, M>::matrix(matrix_multiplication_proxy<MOS...>&& proxy) :
    matrix<T, N, M>(*proxy)
{
}

template<class T, size_t N, size_t M>
template<class... MOS, typename>
matrix<T, N, M>& matrix<T, N, M>::operator=(const matrix_multiplication_proxy<MOS...>& proxy)
{
    *this = *proxy;
    return *this;
}

template<class T, size_t N, size_t M>
template<class... MOS, typename>
matrix<T, N, M>& matrix<T, N, M>::operator=(matrix_multiplication_proxy<MOS...>&& proxy)
{
    *this = *proxy;
    return *this;
}

template<class T, size_t N, size_t M>
template<class TO, size_t NO, size_t MO, typename>
matrix<addition_result_t<T, TO>, smaller<N, NO>, smaller<M, MO>> matrix<T, N, M>::operator+(const matrix<TO, NO, MO>& other) const
{
    matrix<addition_result_t<T, TO>, smaller<N, NO>, smaller<M, MO>> result;

#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(smaller<N, NO>); row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                result._mat[row][column] = _mat[row][column] + other._mat[row][column];
            }
        }
    }
    else
    {
        for (size_t row = 0; row < smaller<N, NO>; row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                result._mat[row][column] = _mat[row][column] + other._mat[row][column];
            }
        }
    }
#else
    for (size_t row = 0; row < smaller<N, NO>; row++)
    {
        for (size_t column = 0; column < smaller<M, MO>; column++)
        {
            result._mat[row][column] = _mat[row][column] + other._mat[row][column];
        }
    }
#endif

    return result;
}

template<class T, size_t N, size_t M>
template<class TO, size_t NO, size_t MO, typename>
matrix<subtraction_result_t<T, TO>, smaller<N, NO>, smaller<M, MO>> matrix<T, N, M>::operator-(const matrix<TO, NO, MO>& other) const
{
    matrix<subtraction_result_t<T, TO>, smaller<N, NO>, smaller<M, MO>> result;

#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(smaller<N, NO>); row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                result._mat[row][column] = _mat[row][column] - other._mat[row][column];
            }
        }
    }
    else
    {
        for (size_t row = 0; row < smaller<N, NO>; row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                result._mat[row][column] = _mat[row][column] - other._mat[row][column];
            }
        }
    }
#else
    for (size_t row = 0; row < smaller<N, NO>; row++)
    {
        for (size_t column = 0; column < smaller<M, MO>; column++)
        {
            result._mat[row][column] = _mat[row][column] - other._mat[row][column];
        }
    }
#endif

    return result;
}

template<class T, size_t N, size_t M>
template<class TO, size_t P, typename>
matrix_multiplication_proxy<matrix<T, N, M>, matrix<TO, M, P>> matrix<T, N, M>::operator*(const matrix<TO, M, P>& other) const
{
    return matrix_multiplication_proxy<matrix<T, N, M>, matrix<TO, M, P>>(std::tuple<const matrix<T, N, M>&, const matrix<TO, M, P>&>(*this, other));
}

template<class T, size_t N, size_t M>
template<class... MOS, typename>
matrix_multiplication_proxy<matrix<T, N, M>, MOS...> matrix<T, N, M>::operator*(const matrix_multiplication_proxy<MOS...>& other) const
{
    return matrix_multiplication_proxy<matrix<T, N, M>, MOS...>(std::tuple_cat(std::tuple<const matrix<T, N, M>&>(*this), other._matrices));
}

template<class T, size_t N, size_t M>
template<class TO, size_t NO, size_t MO, typename>
matrix<T, N, M>& matrix<T, N, M>::operator+=(const matrix<TO, NO, MO>& other)
{
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(smaller<N, NO>); row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] += static_cast<T>(other._mat[row][column]);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < smaller<N, NO>; row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] += static_cast<T>(other._mat[row][column]);
            }
        }
    }
#else
    for (size_t row = 0; row < smaller<N, NO>; row++)
    {
        for (size_t column = 0; column < smaller<M, MO>; column++)
        {
            _mat[row][column] += static_cast<T>(other._mat[row][column]);
        }
    }
#endif

    return *this;
}

template<class T, size_t N, size_t M>
template<class TO, size_t NO, size_t MO, typename>
matrix<T, N, M>& matrix<T, N, M>::operator-=(const matrix<TO, NO, MO>& other)
{
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(smaller<N, NO>); row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] -= static_cast<T>(other._mat[row][column]);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < smaller<N, NO>; row++)
        {
            for (size_t column = 0; column < smaller<M, MO>; column++)
            {
                _mat[row][column] -= static_cast<T>(other._mat[row][column]);
            }
        }
    }
#else
    for (size_t row = 0; row < smaller<N, NO>; row++)
    {
        for (size_t column = 0; column < smaller<M, MO>; column++)
        {
            _mat[row][column] -= static_cast<T>(other._mat[row][column]);
        }
    }
#endif

    return *this;
}

template<class T, size_t N, size_t M>
template<class TO, size_t P, typename>
matrix<T, N, M>& matrix<T, N, M>::operator*=(const matrix<TO, M, P>& other)
{
    *this = std::move(matrix_multiplication_proxy<matrix<T, N, M>, matrix<TO, M, P>>::multiply_matrices(*this, other));
    return *this;
}

template<class T, size_t N, size_t M>
matrix<T, N, M> matrix<T, N, M>::operator-() const
{
    matrix<T, N, M> result;

#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                result._mat[row][column] = -_mat[row][column];
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                result._mat[row][column] = -_mat[row][column];
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            result._mat[row][column] = -_mat[row][column];
        }
    }
#endif
    return result;
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<multiplication_result_t<T, TO>, N, M> matrix<T, N, M>::operator*(const TO& v) const
{
    matrix<multiplication_result_t<T, TO>, N, M> result;

#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                result._mat[row][column] = _mat[row][column] * v;
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                result._mat[row][column] = _mat[row][column] * v;
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            result._mat[row][column] = _mat[row][column] * v;
        }
    }
#endif
    return result;
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<division_result_t<T, TO>, N, M> matrix<T, N, M>::operator/(const TO& v) const
{
    matrix<division_result_t<T, TO>, N, M> result;

#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                result._mat[row][column] = _mat[row][column] / v;
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                result._mat[row][column] = _mat[row][column] / v;
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            result._mat[row][column] = _mat[row][column] / v;
        }
    }
#endif
    return result;
}

template<class T, size_t N, size_t M, class TO, typename = typename std::enable_if_t<can_be_multiplied_v<T, TO> && std::is_convertible_v<TO, T>>>
matrix<multiplication_result_t<T, TO>, N, M> operator*(const TO& v, const matrix<T, N, M>& matrix)
{
    return matrix * v;
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>& matrix<T, N, M>::operator*=(const TO& v)
{
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] *= static_cast<T>(v);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] *= static_cast<T>(v);
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] *= static_cast<T>(v);
        }
    }
#endif
    return *this;
}

template<class T, size_t N, size_t M>
template<class TO, typename>
matrix<T, N, M>& matrix<T, N, M>::operator/=(const TO & v)
{
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] /= static_cast<T>(v);
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] /= static_cast<T>(v);
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            _mat[row][column] /= static_cast<T>(v);
        }
    }
#endif
    return *this;
}

template<class T, size_t N, size_t M>
template<class TO, typename>
vector<inner_product_result_t<T, TO>, N> matrix<T, N, M>::operator*(const vector<TO, M>& vec) const
{
    vector<inner_product_result_t<T, TO>, N> result;

    for (size_t row = 0; row < N; row++)
    {
        inner_product_result_t<T, TO> inner_product = get_additive_identity<inner_product_result_t<T, TO>>();
        for (size_t k = 0; k < N; k++)
        {
            inner_product += vec._coords[k] * _mat[row][k];
        }
        result._coords[row] = inner_product;
    }

    return result;
}

template<class T, size_t N, size_t M>
T* matrix<T, N, M>::operator[](size_t x)
{
    return _mat[x];
}

template<class T, size_t N, size_t M>
const T* matrix<T, N, M>::operator[](size_t x) const
{
    return _mat[x];
}

template<class T, size_t N, size_t M>
T* matrix<T, N, M>::data()
{
    return _mat.data();
}

template<class T, size_t N, size_t M>
const T* matrix<T, N, M>::data() const
{
    return _mat.data();
}

template<class T, size_t N, size_t M>
T* matrix<T, N, M>::begin()
{
    return _mat.data();
}

template<class T, size_t N, size_t M>
const T* matrix<T, N, M>::begin() const
{
    return _mat.data();
}

template<class T, size_t N, size_t M>
T* matrix<T, N, M>::end()
{
    return _mat.data() + N * M;
}

template<class T, size_t N, size_t M>
const T* matrix<T, N, M>::end() const
{
    return _mat.data() + N * M;
}

template<class T, size_t N, size_t M>
template<class TO, size_t NO, size_t MO, typename>
bool matrix<T, N, M>::operator==(const matrix<TO, NO, MO>& other) const
{
    if (NO == N && MO == M)
    {
#if USE_OPENMP
        if (is_big_matrix)
        {
            bool equals = true;

#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N); row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    if (!equal(_mat[row][column], other._mat[row][column]))
                    {
                        equals = false;
                    }
                }
            }

            return equals;
        }
        else
        {
            for (size_t row = 0; row < N; row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    if (!equal(_mat[row][column], other._mat[row][column]))
                    {
                        return false;
                    }
                }
            }
        }
#else
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                if (!equal(_mat[row][column], other._mat[row][column]))
                {
                    return false;
                }
            }
        }
#endif
        return true;
    }
    else
    {
        return false;
    }
}

template<class T, size_t N, size_t M>
template<class TO, size_t NO, size_t MO, typename>
bool matrix<T, N, M>::operator!=(const matrix<TO, NO, MO>& other) const
{
    if (NO == N && MO == M)
    {
#if USE_OPENMP
        if (is_big_matrix)
        {
            bool inequals = false;

#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N); row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    if (inequal(_mat[row][column], other._mat[row][column]))
                    {
                        inequals = true;
                    }
                }
            }

            return inequals;
        }
        else
        {
            for (size_t row = 0; row < N; row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    if (inequal(_mat[row][column], other._mat[row][column]))
                    {
                        return true;
                    }
                }
            }
        }
#else
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                if (inequal(_mat[row][column], other._mat[row][column]))
                {
                    return true;
                }
            }
        }
#endif
        return false;
    }
    else
    {
        return true;
    }
}

template<class T, size_t N, size_t M>
template<class TO, size_t NO, size_t MO>
inline void matrix<T, N, M>::swap_rows(matrix<TO, NO, MO>& matrix, size_t r1, size_t r2, size_t start_column, size_t end_column)
{
    for (size_t column = start_column; column < end_column; column++)
    {
        std::swap(matrix._mat[r1][column], matrix._mat[r2][column]);
    }
}

template<class T, size_t N, size_t M>
template<class TO, size_t NO, size_t MO>
inline void matrix<T, N, M>::swap_columns(matrix<TO, NO, MO>& matrix, size_t c1, size_t c2, size_t start_row, size_t end_row)
{
    for (size_t row = start_row; row < end_row; row++)
    {
        std::swap(matrix._mat[row][c1], matrix._mat[row][c2]);
    }
}

template<class T, size_t N, size_t M>
template<typename>
T matrix<T, N, M>::determinant() const
{
    constexpr size_t S = N;

    auto copy = *this;

    //gaussian algorithm is used to turn this matrix into triangular matrix
    //in order to efficently calculate determinant

    T determinant_sign = get_multiplicative_identity<T>;
    T determinant_value = get_multiplicative_identity<T>;

    for (size_t diagonal = 0; diagonal < S; diagonal++)
    {
        //if current diagonal element is equal to 0 we are trying to find row below diagonal row in which
        //element on current diagonal column is not equal to 0

        if (equal(copy._mat[diagonal][diagonal], get_additive_identity<T>()))
        {
            size_t non_zero_row = diagonal;

            for (size_t row = diagonal + 1; row < S; row++)
            {
                if (!equal(copy._mat[row][diagonal], get_additive_identity<T>()))
                {
                    non_zero_row = row;
                    break;
                }
            }

            if (non_zero_row != diagonal)
            {
                //if non zero row is found we swap current diagonal row with found row
                //and change the sign of determinant
                swap_rows(copy, diagonal, non_zero_row, diagonal);

                determinant_sign = -determinant_sign;
            }
            else
            {
                //if non zero row is not found then it implies that matrix determinant is equal to 0
                return get_additive_identity<T>();
            }
        }

        //subtracting this row from rows below in order to obtain 0 in elements
        //below current diagonal element
#if USE_OPENMP
        if (is_big_matrix)
        {
#pragma omp parallel for
            for (int row = static_cast<int>(diagonal + 1); row < static_cast<int>(S); row++)
            {
                //factor from equation -> 0 = matrix[row][diagonal] - factor*matrix[diagonal][diagonal] (0 is what we want to get on this element)
                T factor = copy._mat[row][diagonal] / copy._mat[diagonal][diagonal];

                //subtracting diagonal row multiplied by factor from current row
                //elements with column < diagonal are already 0 from previous iterations
                for (size_t column = diagonal; column < S; column++)
                {
                    copy._mat[row][column] -= copy._mat[diagonal][column] * factor;
                }
            }
        }
        else
        {
            for (size_t row = diagonal + 1; row < S; row++)
            {
                //factor from equation -> 0 = matrix[row][diagonal] - factor*matrix[diagonal][diagonal] (0 is what we want to get on this element)
                T factor = copy._mat[row][diagonal] / copy._mat[diagonal][diagonal];

                //subtracting diagonal row multiplied by factor from current row
                //elements with column < diagonal are already 0 from previous iterations
                for (size_t column = diagonal; column < S; column++)
                {
                    copy._mat[row][column] -= copy._mat[diagonal][column] * factor;
                }
            }
        }
#else
        for (size_t row = diagonal + 1; row < S; row++)
        {
            //factor from equation -> 0 = matrix[row][diagonal] - factor*matrix[diagonal][diagonal] (0 is what we want to get on this element)
            T factor = copy._mat[row][diagonal] / copy._mat[diagonal][diagonal];

            //subtracting diagonal row multiplied by factor from current row
            //elements with column < diagonal are already 0 from previous iterations
            for (size_t column = diagonal; column < S; column++)
            {
                copy._mat[row][column] -= copy._mat[diagonal][column] * factor;
            }
        }
#endif
        determinant_value *= copy._mat[diagonal][diagonal];
    }
    return determinant_value * determinant_sign;
}

template<class T, size_t N, size_t M>
template<typename>
std::optional<matrix<T, N, M>> matrix<T, N, M>::inverted() const
{
    constexpr size_t S = N;

    //trying to convert this matrix to identity matrix
    //by gaussian algorithm to obtain inverse matrix

    matrix<T, S, S * 2> copy(*this);

    //right part of the larger matrix is identity matrix
    //so we need to initialize its diagonal elements to 1
    for (size_t diagonal = 0; diagonal < S; diagonal++)
    {
        copy._mat[diagonal][S + diagonal] = get_multiplicative_identity<T>();
    }

    for (size_t diagonal = 0; diagonal < S; diagonal++)
    {
        //if current diagonal element is equal to 0 we are trying to find row below diagonal row in which
        //element on current diagonal column is not equal to 0

        if (equal(copy._mat[diagonal][diagonal], get_additive_identity<T>()))
        {
            size_t non_zero_row = diagonal;

            for (size_t row = diagonal + 1; row < S; row++)
            {
                if (!equal(copy._mat[row][diagonal], get_additive_identity<T>()))
                {
                    non_zero_row = row;
                    break;
                }
            }

            if (non_zero_row != diagonal)
            {
                //if non zero row is found we swap current diagonal row with found row
                swap_rows(copy, diagonal, non_zero_row, diagonal);
            }
            else
            {
                //if non zero row is not found then it implies that matrix has no inverse
                return std::nullopt;
            }
        }

        //dividing diagonal row by diagonal element to obtain 1 on diagonal element
        T factor = copy._mat[diagonal][diagonal];
        for (size_t column = diagonal; column < S * 2; column++)
        {
            copy._mat[diagonal][column] /= factor;
        }

#if USE_OPENMP
        if (is_big_matrix)
        {
            //subtracting this row from rows above in order to obtain 0 in elements
            //above current diagonal element
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(diagonal); row++)
            {
                //factor from equation -> 0 = matrix[row][diagonal] - factor*matrix[diagonal][diagonal] (0 is what we want to get on this element)
                T factor = copy._mat[row][diagonal] / copy._mat[diagonal][diagonal];

                //subtracting diagonal row multiplied by factor from current row
                //elements with column < diagonal are already 0 from previous iterations
                for (size_t column = diagonal; column < S * 2; column++)
                {
                    copy._mat[row][column] -= copy._mat[diagonal][column] * factor;
                }
            }
            //subtracting this row from rows below in order to obtain 0 in elements
            //below current diagonal element
#pragma omp parallel for
            for (int row = static_cast<int>(diagonal + 1); row < static_cast<int>(S); row++)
            {
                //factor from equation -> 0 = matrix[row][diagonal] - factor*matrix[diagonal][diagonal] (0 is what we want to get on this element)
                T factor = copy._mat[row][diagonal] / copy._mat[diagonal][diagonal];

                //subtracting diagonal row multiplied by factor from current row
                //elements with column < diagonal are already 0 from previous iterations
                for (size_t column = diagonal; column < S * 2; column++)
                {
                    copy._mat[row][column] -= copy._mat[diagonal][column] * factor;
                }
            }
        }
        else
        {
            //subtracting this row from rows above in order to obtain 0 in elements
            //above current diagonal element
            for (size_t row = 0; row < diagonal; row++)
            {
                //factor from equation -> 0 = matrix[row][diagonal] - factor*matrix[diagonal][diagonal] (0 is what we want to get on this element)
                T factor = copy._mat[row][diagonal] / copy._mat[diagonal][diagonal];

                //subtracting diagonal row multiplied by factor from current row
                //elements with column < diagonal are already 0 from previous iterations
                for (size_t column = diagonal; column < S * 2; column++)
                {
                    copy._mat[row][column] -= copy._mat[diagonal][column] * factor;
                }
            }
            //subtracting this row from rows below in order to obtain 0 in elements
            //below current diagonal element
            for (size_t row = diagonal + 1; row < S; row++)
            {
                //factor from equation -> 0 = matrix[row][diagonal] - factor*matrix[diagonal][diagonal] (0 is what we want to get on this element)
                T factor = copy._mat[row][diagonal] / copy._mat[diagonal][diagonal];

                //subtracting diagonal row multiplied by factor from current row
                //elements with column < diagonal are already 0 from previous iterations
                for (size_t column = diagonal; column < S * 2; column++)
                {
                    copy._mat[row][column] -= copy._mat[diagonal][column] * factor;
                }
            }
        }
#else
        //subtracting this row from rows above in order to obtain 0 in elements
        //above current diagonal element
        for (size_t row = 0; row < diagonal; row++)
        {
            //factor from equation -> 0 = matrix[row][diagonal] - factor*matrix[diagonal][diagonal] (0 is what we want to get on this element)
            T factor = copy._mat[row][diagonal] / copy._mat[diagonal][diagonal];

            //subtracting diagonal row multiplied by factor from current row
            //elements with column < diagonal are already 0 from previous iterations
            for (size_t column = diagonal; column < S * 2; column++)
            {
                copy._mat[row][column] -= copy._mat[diagonal][column] * factor;
            }
        }
        //subtracting this row from rows below in order to obtain 0 in elements
        //below current diagonal element
        for (size_t row = diagonal + 1; row < S; row++)
        {
            //factor from equation -> 0 = matrix[row][diagonal] - factor*matrix[diagonal][diagonal] (0 is what we want to get on this element)
            T factor = copy._mat[row][diagonal] / copy._mat[diagonal][diagonal];

            //subtracting diagonal row multiplied by factor from current row
            //elements with column < diagonal are already 0 from previous iterations
            for (size_t column = diagonal; column < S * 2; column++)
            {
                copy._mat[row][column] -= copy._mat[diagonal][column] * factor;
            }
        }
#endif
    }

    //right part of the larger matrix is now our inverse to this matrix
    matrix<T, S, S> result;

    for (size_t row = 0; row < S; row++)
    {
        for (size_t column = 0; column < S; column++)
        {
            result._mat[row][column] = copy._mat[row][column + S];
        }
    }

    return std::optional<matrix<T, N, M>>(result);
}

template<class T, size_t N, size_t M>
size_t matrix<T, N, M>::rank() const
{
    auto copy = *this;

    if constexpr (N <= M)
    {
        //there is less or equal rows than column, it means that rank of this matrix can be at max equal to row count

        size_t rank = N;
        for (size_t diagonal = 0; diagonal < rank; diagonal++)
        {
            if (!equal(copy._mat[diagonal][diagonal], get_additive_identity<T>()))
            {
                //if current diagonal element is not equal to zero, we subtract current diagonal row
                //from all rows below it to obtain 0-oes on elements below current diagonal element
                for (size_t row = diagonal + 1; row < rank; row++)
                {
                    //factor from equation -> 0 = matrix[row][diagonal] - factor*matrix[diagonal][diagonal] (0 is what we want to get on this element)
                    T factor = copy._mat[row][diagonal] / copy._mat[diagonal][diagonal];

                    //subtracting diagonal row multiplied by factor from current row
                    //elements with column < diagonal are already 0 from previous iterations
                    for (size_t column = diagonal; column < M; column++)
                    {
                        copy._mat[row][column] -= copy._mat[diagonal][column] * factor;
                    }
                }
            }
            else
            {
                //if current diagonal element is equal to zero,we are trying to find non-zero
                //element below current diagonal element

                bool reduce_rank = true;
                for (size_t row = diagonal + 1; row < N; row++)
                {
                    if (!equal(copy._mat[row][diagonal], get_additive_identity<T>()))
                    {
                        //row is found, we do not need to reduce matrix rank
                        reduce_rank = false;
                        swap_rows(copy, diagonal, row, diagonal, rank);
                        break;
                    }
                }

                if (reduce_rank)
                {
                    //copying last column to current diagonal column
                    for (size_t row = 0; row < N; row++)
                    {
                        copy._mat[row][diagonal] = copy._mat[row][M - N + rank - 1];
                    }

                    //reduce rank
                    rank--;

                    //process this row again
                    diagonal--;
                    continue;
                }
            }
        }

        return rank;
    }
    if constexpr (N > M)
    {
        size_t rank = M;
        for (size_t diagonal = 0; diagonal < rank; diagonal++)
        {
            if (!equal(copy._mat[diagonal][diagonal], get_additive_identity<T>()))
            {
                //if current diagonal element is not equal to zero, we subtract current diagonal column
                //from all columns to the right it to obtain 0-oes on elements to the right of current diagonal element
                for (size_t column = diagonal + 1; column < rank; column++)
                {
                    //factor from equation -> 0 = matrix[row][diagonal] - factor*matrix[diagonal][diagonal] (0 is what we want to get on this element)
                    T factor = copy._mat[diagonal][column] / copy._mat[diagonal][diagonal];

                    //subtracting diagonal column multiplied by factor from current column
                    //elements with row < diagonal are already 0 from previous iterations
                    for (size_t row = diagonal; row < N; row++)
                    {
                        copy._mat[row][column] -= copy._mat[row][diagonal] * factor;
                    }
                }
            }
            else
            {
                //if current diagonal element is equal to zero,we are trying to find non-zero
                //element to the right of current diagonal element

                bool reduce_rank = true;
                for (size_t column = diagonal + 1; column < M; column++)
                {
                    if (!equal(copy._mat[diagonal][column], get_additive_identity<T>()))
                    {
                        //column is found, we do not need to reduce matrix rank
                        reduce_rank = false;
                        swap_columns(copy, diagonal, column, diagonal, rank);
                        break;
                    }
                }

                if (reduce_rank)
                {
                    //copying last row to current diagonal row
                    for (size_t column = 0; column < M; column++)
                    {
                        copy._mat[diagonal][column] = copy._mat[N - M + rank - 1][column];
                    }

                    //reduce rank
                    rank--;

                    //process this column again
                    diagonal--;
                    continue;
                }
            }
        }

        return rank;
    }
}

template<class T, size_t N, size_t M>
matrix<T, M, N> matrix<T, N, M>::transposed() const
{
    matrix<T, M, N> result;

    //switching columns with rows
#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                result._mat[column][row] = _mat[row][column];
            }
        }
    }
    else
    {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                result._mat[column][row] = _mat[row][column];
            }
        }
    }
#else
    for (size_t row = 0; row < N; row++)
    {
        for (size_t column = 0; column < M; column++)
        {
            result._mat[column][row] = _mat[row][column];
        }
    }
#endif

    return result;
}

template<class T, size_t N, size_t M>
template<typename>
matrix<T, M, N>& matrix<T, N, M>::transpose()
{
    *this = transposed();
    return *this;
}

template<class T, size_t N, size_t M>
template<typename>
const matrix<T, N, M>& matrix<T, N, M>::MULTIPLICATIVE_IDENTITY()
{
    constexpr size_t S = N;

    static matrix<T, S, S>  _identity;
    static bool             _initialized = false;
    if (!_initialized)
    {
        for (size_t diagonal = 0; diagonal < S; diagonal++)
        {
            _identity._mat[diagonal][diagonal] = get_multiplicative_identity<T>;
        }
        _initialized = true;
    }
    return _identity;
}

template<class T, size_t N, size_t M>
const matrix<T, N, M>& matrix<T, N, M>::ADDITIVE_IDENTITY()
{
    constexpr size_t S = N;

    static matrix<T, N, M> _identity;

    return _identity;
}

NAMESPACE_LINEAR_ALGEBRA_IO_BEGIN

template<class TO, size_t NO, size_t MO>
std::ostream& operator<<(std::ostream& os, const matrix<TO, NO, MO>& m)
{
    for (size_t row = 0; row < NO - 1; row++)
    {
        for (size_t column = 0; column < MO - 1; column++)
        {
            os << m._mat[row][column] << ",\t";
        }

        os << m._mat[row][MO - 1];

        os << std::endl;
    }

    for (size_t column = 0; column < MO - 1; column++)
    {
        os << m._mat[NO - 1][column] << ",\t";
    }

    os << m._mat[NO - 1][MO - 1];

    return os;
}

NAMESPACE_LINEAR_ALGEBRA_IO_END

NAMESPACE_LINEAR_ALGEBRA_END