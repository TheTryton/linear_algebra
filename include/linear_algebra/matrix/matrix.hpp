#pragma once

#include "../linear_algebra_common.hpp"

NAMESPACE_LINEAR_ALGEBRA_BEGIN

template<class... MS>
class matrix_multiplication_proxy;

template<class M, class... MS>
constexpr size_t matrix_multiplication_proxy_result_rows()
{
    return M::rows();
}

template<class M>
constexpr size_t matrix_multiplication_proxy_result_columns()
{
    return M::columns();
}

template<class M1, class M2, class... MS>
constexpr size_t matrix_multiplication_proxy_result_columns()
{
    return matrix_multiplication_proxy_result_columns<M2, MS...>();
}

template<class T, size_t N, size_t M>
class matrix
{
    static_assert(N != 0 && M != 0, "Matrix dimensions must be at least 1!");
    static_assert(std::is_arithmetic_v<T>, "Matrix element type must be a scalar!");
    static_assert(std::is_default_constructible_v<T>, "Matrix element type must be default constructible!");
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
        inline const T* operator[](size_t row) const
        {
            return _mat[row];
        }
        inline T* operator[](size_t row)
        {
            return _mat[row];
        }
        inline const T* data() const
        {
            return _mat;
        }
        inline T* data()
        {
            return _mat;
        }
    };

    class matrix_storage_dynamic
    {
    private:
        T* _mat = nullptr;
    public:
        matrix_storage_dynamic()
        {
            _mat = new T[N*M];
        }
        matrix_storage_dynamic(matrix_storage_dynamic&& other)
        {
            _mat = other._mat;
            other._mat = nullptr;
        }
        matrix_storage_dynamic& operator=(matrix_storage_dynamic&& other)
        {
            _mat = other._mat;
            other._mat = nullptr;
            return *this;
        }
        ~matrix_storage_dynamic()
        {
            if (_mat)
            {
                delete[] _mat;
            }
        }
    public:
        inline const T* operator[](size_t row) const
        {
            return _mat + row * M;
        }
        inline T* operator[](size_t row)
        {
            return _mat + row * M;
        }
        inline const T* data() const
        {
            return _mat;
        }
        inline T* data()
        {
            return _mat;
        }
    };

    static constexpr bool is_big_matrix = N * M * sizeof(T) >= 80000;

    using storage_type = typename std::conditional_t<is_big_matrix, matrix_storage_dynamic, matrix_storage_static>;
    storage_type _mat;
private:

public:
    using calculation_type = T;
public:
    //default constructor

    matrix()
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
                    _mat[row][column] = static_cast<T>(0);
                }
            }
        }
        else
        {
            for (size_t row = 0; row < N; row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    _mat[row][column] = static_cast<T>(0);
                }
            }
        }
#else
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(0);
            }
        }
#endif
    }

    //copy, move constructors and operators

    //same type, same dimensions

    matrix(const matrix<T, N, M>& other)
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

    matrix(matrix<T, N, M>&& other)
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

    matrix<T, N, M>& operator=(const matrix<T, N, M>& other)
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

    matrix<T, N, M>& operator=(matrix<T, N, M>&& other)
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

    //different type, same dimension

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(const matrix<TO, N, M>& other)
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

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(matrix<TO, N, M>&& other)
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

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(const matrix<TO, N, M>& other)
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
    
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(matrix<TO, N, M>&& other)
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

    //different type, different dimensions

    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(const matrix<TO, NO, MO>& other)
    {
        constexpr size_t N_min = NO > N ? N : NO;
        constexpr size_t M_min = MO > M ? M : MO;
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
            for (int row = 0; row < static_cast<int>(N_min); row++)
            {
                for (size_t column = 0; column < M_min; column++)
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
            for (int row = static_cast<int>(N_min); row < static_cast<int>(N); row++)
            {
                for (size_t column = 0; column < M_min; column++)
                {
                    _mat[row][column] = static_cast<T>(0);
                }
            }

            //  filling right-top submatrix to initialize it with 0-oes
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N_min); row++)
            {
                for (size_t column = M_min; column < M; column++)
                {
                    _mat[row][column] = static_cast<T>(0);
                }
            }

            //  filling bottom-right submatrix to initialize it with 0-oes
#pragma omp parallel for
            for (int row = static_cast<int>(N_min); row < static_cast<int>(N); row++)
            {
                for (size_t column = M_min; column < M; column++)
                {
                    _mat[row][column] = static_cast<T>(0);
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

            for (size_t row = 0; row < N_min; row++)
            {
                for (size_t column = 0; column < M_min; column++)
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
            for (size_t row = N_min; row < N; row++)
            {
                for (size_t column = 0; column < M_min; column++)
                {
                    _mat[row][column] = static_cast<T>(0);
                }
            }

            //  filling right-top submatrix to initialize it with 0-oes
            for (size_t row = 0; row < N_min; row++)
            {
                for (size_t column = M_min; column < M; column++)
                {
                    _mat[row][column] = static_cast<T>(0);
                }
            }

            //  filling bottom-right submatrix to initialize it with 0-oes
            for (size_t row = N_min; row < N; row++)
            {
                for (size_t column = M_min; column < M; column++)
                {
                    _mat[row][column] = static_cast<T>(0);
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

        for (size_t row = 0; row < N_min; row++)
        {
            for (size_t column = 0; column < M_min; column++)
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
        for (size_t row = N_min; row < N; row++)
        {
            for (size_t column = 0; column < M_min; column++)
            {
                _mat[row][column] = static_cast<T>(0);
            }
        }

        //  filling right-top submatrix to initialize it with 0-oes
        for (size_t row = 0; row < N_min; row++)
        {
            for (size_t column = M_min; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(0);
            }
        }

        //  filling bottom-right submatrix to initialize it with 0-oes
        for (size_t row = N_min; row < N; row++)
        {
            for (size_t column = M_min; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(0);
            }
        }
#endif
    }

    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(matrix<TO, NO, MO>&& other)
    {
        constexpr size_t N_min = NO > N ? N : NO;
        constexpr size_t M_min = MO > M ? M : MO;
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
            for (int row = 0; row < static_cast<int>(N_min); row++)
            {
                for (size_t column = 0; column < M_min; column++)
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
            for (int row = static_cast<int>(N_min); row < static_cast<int>(N); row++)
            {
                for (size_t column = 0; column < M_min; column++)
                {
                    _mat[row][column] = static_cast<T>(0);
                }
            }

            //  filling right-top submatrix to initialize it with 0-oes
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N_min); row++)
            {
                for (size_t column = M_min; column < M; column++)
                {
                    _mat[row][column] = static_cast<T>(0);
                }
            }

            //  filling bottom-right submatrix to initialize it with 0-oes
#pragma omp parallel for
            for (int row = static_cast<int>(N_min); row < static_cast<int>(N); row++)
            {
                for (size_t column = M_min; column < M; column++)
                {
                    _mat[row][column] = static_cast<T>(0);
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

            for (size_t row = 0; row < N_min; row++)
            {
                for (size_t column = 0; column < M_min; column++)
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
            for (size_t row = N_min; row < N; row++)
            {
                for (size_t column = 0; column < M_min; column++)
                {
                    _mat[row][column] = static_cast<T>(0);
                }
            }

            //  filling right-top submatrix to initialize it with 0-oes
            for (size_t row = 0; row < N_min; row++)
            {
                for (size_t column = M_min; column < M; column++)
                {
                    _mat[row][column] = static_cast<T>(0);
                }
            }

            //  filling bottom-right submatrix to initialize it with 0-oes
            for (size_t row = N_min; row < N; row++)
            {
                for (size_t column = M_min; column < M; column++)
                {
                    _mat[row][column] = static_cast<T>(0);
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

        for (size_t row = 0; row < N_min; row++)
        {
            for (size_t column = 0; column < M_min; column++)
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
        for (size_t row = N_min; row < N; row++)
        {
            for (size_t column = 0; column < M_min; column++)
            {
                _mat[row][column] = static_cast<T>(0);
            }
        }

        //  filling right-top submatrix to initialize it with 0-oes
        for (size_t row = 0; row < N_min; row++)
        {
            for (size_t column = M_min; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(0);
            }
        }

        //  filling bottom-right submatrix to initialize it with 0-oes
        for (size_t row = N_min; row < N; row++)
        {
            for (size_t column = M_min; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(0);
            }
        }
#endif
    }

    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(const matrix<TO, NO, MO>& other)
    {
        constexpr size_t N_min = NO > N ? N : NO;
        constexpr size_t M_min = MO > M ? M : MO;
#if USE_OPENMP
        if (is_big_matrix)
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N_min); row++)
            {
                for (size_t column = 0; column < M_min; column++)
                {
                    _mat[row][column] = static_cast<T>(other._mat[row][column]);
                }
            }
        }
        else
        {
            for (size_t row = 0; row < N_min; row++)
            {
                for (size_t column = 0; column < M_min; column++)
                {
                    _mat[row][column] = static_cast<T>(other._mat[row][column]);
                }
            }
        }
#else
        for (size_t row = 0; row < N_min; row++)
        {
            for (size_t column = 0; column < M_min; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
#endif
        return *this;
    }

    template<class TO, size_t NO, size_t MO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(matrix<TO, NO, MO>&& other)
    {
        constexpr size_t N_min = NO > N ? N : NO;
        constexpr size_t M_min = MO > M ? M : MO;
#if USE_OPENMP
        if (is_big_matrix)
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N_min); row++)
            {
                for (size_t column = 0; column < M_min; column++)
                {
                    _mat[row][column] = static_cast<T>(other._mat[row][column]);
                }
            }
        }
        else
        {
            for (size_t row = 0; row < N_min; row++)
            {
                for (size_t column = 0; column < M_min; column++)
                {
                    _mat[row][column] = static_cast<T>(other._mat[row][column]);
                }
            }
        }
#else
        for (size_t row = 0; row < N_min; row++)
        {
            for (size_t column = 0; column < M_min; column++)
            {
                _mat[row][column] = static_cast<T>(other._mat[row][column]);
            }
        }
#endif
        return *this;
    }

    //regular constructors

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(const T& v)
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

    template<class... TOS, typename = typename std::enable_if_t<(sizeof...(TOS) > 1) && (std::is_convertible_v<TOS, T> && ...)>>
    matrix(const TOS&... vs)
    {
        std::array<T, sizeof...(TOS)> elements = { static_cast<T>(vs)... };

        constexpr size_t DM = sizeof...(T) > N*M ? N * M : sizeof...(T);

        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                if(row*column < sizeof...(T))
                {
                    _mat[row][column] = elements[row*M + column];
                }
                else
                {
                    _mat[row][column] = static_cast<T>(0);
                }
            }
        }
    }

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix(TO&& v)
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
    
    template<class... TOS, typename = typename std::enable_if_t<(sizeof...(TOS) > 1) && (std::is_convertible_v<TOS, T> && ...)>>
    matrix(TOS&&... vs)
    {
        std::array<T, sizeof...(TOS)> elements = { static_cast<T>(vs)... };

        constexpr size_t DM = sizeof...(T) > N*M ? N * M : sizeof...(T);

        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                if (row*column < sizeof...(T))
                {
                    _mat[row][column] = std::move(elements[row*M + column]);
                }
                else
                {
                    _mat[row][column] = static_cast<T>(0);
                }
            }
        }
    }

    matrix(std::initializer_list<T> vs)
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
                _mat[row][column] = static_cast<T>(0);
            }
        }
    }
    
    matrix(std::initializer_list<std::initializer_list<T>> vs)
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
                if(row_elements == row_elements_end)
                {
                    for (; column < M; column++)
                    {
                        _mat[row][column] = static_cast<T>(0);
                    }
                }
            }
        }

        //every other row is set to 0

        for (; row < N; row++, rows++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] = static_cast<T>(0);
            }
        }
    }

    //regular operators

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(const TO& v)
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

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    matrix<T, N, M>& operator=(TO&& v)
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
    
    matrix<T, N, M>& operator=(std::initializer_list<T> vs)
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
    
    matrix<T, N, M>& operator=(std::initializer_list<std::initializer_list<T>> vs)
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

    //from proxy
    template<class... MS, typename = typename std::enable_if_t<matrix_multiplication_proxy_result_rows<MS...>() == N && matrix_multiplication_proxy_result_columns<MS...>() == M>>
    matrix<T, N, M>(const matrix_multiplication_proxy<MS...>& proxy);
    template<class... MS, typename = typename std::enable_if_t<matrix_multiplication_proxy_result_rows<MS...>() == N && matrix_multiplication_proxy_result_columns<MS...>() == M>>
    matrix<T, N, M>& operator=(const matrix_multiplication_proxy<MS...>& proxy);
public:
    //matrix-matrix operators

    template<class TO, size_t NO, size_t MO>
    auto operator+(const matrix<TO, NO, MO>& other) const
    {
        constexpr size_t N_max = NO > N ? NO : N;
        constexpr size_t M_max = MO > M ? MO : M;

        using cm_t = decltype(std::declval<T>() + std::declval<TO>());

        matrix<cm_t, N_max, M_max> result(*this);
#if USE_OPENMP
        if(is_big_matrix)
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(NO); row++)
            {
                for (size_t column = 0; column < MO; column++)
                {
                    result._mat[row][column] += other._mat[row][column];
                }
            }
        }
        else
        {
            for (size_t row = 0; row < NO; row++)
            {
                for (size_t column = 0; column < MO; column++)
                {
                    result._mat[row][column] += other._mat[row][column];
                }
                }
            }
#else
        for (size_t row = 0; row < NO; row++)
        {
            for (size_t column = 0; column < MO; column++)
            {
                result._mat[row][column] += other._mat[row][column];
            }
        }
#endif
        return result;
    }
    
    template<class TO, size_t NO, size_t MO>
    auto operator-(const matrix<TO, NO, MO>& other) const
    {
        constexpr size_t N_max = NO > N ? NO : N;
        constexpr size_t M_max = MO > M ? MO : M;

        using cm_t = decltype(std::declval<T>() + std::declval<TO>());

        matrix<cm_t, N_max, M_max> result(*this);

#if USE_OPENMP
        if(is_big_matrix)
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(NO); row++)
            {
                for (size_t column = 0; column < MO; column++)
                {
                    result._mat[row][column] -= other._mat[row][column];
                }
            }
        }
        else
        {
            for (size_t row = 0; row < NO; row++)
            {
                for (size_t column = 0; column < MO; column++)
                {
                    result._mat[row][column] -= other._mat[row][column];
                }
            }
        }
#else
        for (size_t row = 0; row < NO; row++)
        {
            for (size_t column = 0; column < MO; column++)
            {
                result._mat[row][column] -= other._mat[row][column];
            }
        }
#endif

        return result;
    }
    
    template<class TO, size_t P>
    auto operator*(const matrix<TO, M, P>& other) const
    {
        using cm_t = decltype(std::declval<T>() * std::declval<TO>() + std::declval<T>() * std::declval<TO>());

        matrix<cm_t, N, P> result;

#if USE_OPENMP
        if(is_big_matrix)
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N); row++)
            {
                for (size_t column = 0; column < P; column++)
                {
                    //calculating column-row inner product

                    cm_t inner_product = static_cast<cm_t>(0);

                    for (size_t k = 0; k < M; k++)
                    {
                        inner_product += static_cast<cm_t>(_mat[row][k] * other._mat[k][column]);
                    }

                    result._mat[row][column] = inner_product;
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

                    cm_t inner_product = static_cast<cm_t>(0);

                    for (size_t k = 0; k < M; k++)
                    {
                        inner_product += static_cast<cm_t>(_mat[row][k] * other._mat[k][column]);
                    }

                    result._mat[row][column] = inner_product;
                }
            }
        }
#else
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < P; column++)
            {
                //calculating column-row inner product

                cm_t inner_product = static_cast<cm_t>(0);

                for (size_t k = 0; k < M; k++)
                {
                    inner_product += static_cast<cm_t>(_mat[row][k] * other._mat[k][column]);
                }

                result._mat[row][column] = inner_product;
            }
        }
#endif

        return result;
    }

    template<class TO, size_t NO, size_t MO>
    matrix<T, N, M>& operator+=(const matrix<TO, NO, MO>& other)
    {
        constexpr size_t N_min = NO > N ? N : NO;
        constexpr size_t M_min = MO > M ? M : MO;

#if USE_OPENMP
        if(is_big_matrix)
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N_min); row++)
            {
                for (size_t column = 0; column < M_min; column++)
                {
                    _mat[row][column] += static_cast<T>(other._mat[row][column]);
                }
            }
        }
        else
        {
            for (size_t row = 0; row < N_min; row++)
            {
                for (size_t column = 0; column < M_min; column++)
                {
                    _mat[row][column] += static_cast<T>(other._mat[row][column]);
                }
                }
            }
#else
        for (size_t row = 0; row < N_min; row++)
        {
            for (size_t column = 0; column < M_min; column++)
            {
                _mat[row][column] += static_cast<T>(other._mat[row][column]);
            }
        }
#endif

        return *this;
    }
    
    template<class TO, size_t NO, size_t MO>
    matrix<T, N, M>& operator-=(const matrix<TO, NO, MO>& other)
    {
        constexpr size_t N_min = NO > N ? N : NO;
        constexpr size_t M_min = MO > M ? M : MO;

#if USE_OPENMP
        if(is_big_matrix)
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N_min); row++)
            {
                for (size_t column = 0; column < M_min; column++)
                {
                    _mat[row][column] -= static_cast<T>(other._mat[row][column]);
                }
            }
        }
        else
        {
            for (size_t row = 0; row < N_min; row++)
            {
                for (size_t column = 0; column < M_min; column++)
                {
                    _mat[row][column] -= static_cast<T>(other._mat[row][column]);
                }
            }
        }
#else
        for (size_t row = 0; row < N_min; row++)
        {
            for (size_t column = 0; column < M_min; column++)
            {
                _mat[row][column] -= static_cast<T>(other._mat[row][column]);
            }
        }
#endif

        return *this;
    }
    
    template<class TO, size_t P, typename = typename std::enable_if_t<M == P>>
    matrix<T, N, M>& operator*=(const matrix<TO, M, P>& other)
    {
        *this = std::move(operator*(other));
        return *this;
    }

    //matrix-scalar operators

    matrix<T, N, M> operator-() const
    {
        matrix<T, N, M> result(*this);

#if USE_OPENMP
        if (is_big_matrix)
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N); row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    result._mat[row][column] *= static_cast<T>(-1);
                }
            }
        }
        else
        {
            for (size_t row = 0; row < N; row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    result._mat[row][column] *= static_cast<T>(-1);
                }
            }
        }
#else
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                result._mat[row][column] *= static_cast<T>(-1);
            }
        }
#endif
        return result;
    }

    template<class TO, typename = typename std::enable_if_t<std::is_arithmetic_v<TO>>>
    auto operator*(const TO& v) const
    {
        using cm_t = decltype(std::declval<T>()*std::declval<TO>());

        matrix<cm_t, N, M> result(*this);

#if USE_OPENMP
        if (is_big_matrix)
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N); row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    result._mat[row][column] *= v;
                }
            }
        }
        else
        {
            for (size_t row = 0; row < N; row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    result._mat[row][column] *= v;
                }
            }
        }
#else
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                result._mat[row][column] *= v;
            }
        }
#endif
        return result;
    }
    
    template<class TO, typename = typename std::enable_if_t<std::is_arithmetic_v<TO>>>
    auto operator/(const TO& v) const
    {
        using cm_t = decltype(std::declval<T>()*std::declval<TO>());

        matrix<cm_t, N, M> result(*this);

#if USE_OPENMP
        if (is_big_matrix)
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N); row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    result._mat[row][column] /= v;
                }
            }
        }
        else
        {
            for (size_t row = 0; row < N; row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    result._mat[row][column] /= v;
                }
            }
        }
#else
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                result._mat[row][column] /= v;
            }
        }
#endif
        return result;
    }

    template<class TO, typename = typename std::enable_if_t<std::is_arithmetic_v<TO>>>
    matrix<T, N, M>& operator*=(const TO& v)
    {
#if USE_OPENMP
        if (is_big_matrix)
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N); row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    _mat[row][column] *= v;
                }
            }
        }
        else
        {
            for (size_t row = 0; row < N; row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    _mat[row][column] *= v;
                }
            }
        }
#else
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] *= v;
            }
        }
#endif
        return *this;
    }
    template<class TO, typename = typename std::enable_if_t<std::is_arithmetic_v<TO>>>
    matrix<T, N, M>& operator/=(const TO& v)
    {
#if USE_OPENMP
        if (is_big_matrix)
        {
#pragma omp parallel for
            for (int row = 0; row < static_cast<int>(N); row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    _mat[row][column] /= v;
                }
            }
        }
        else
        {
            for (size_t row = 0; row < N; row++)
            {
                for (size_t column = 0; column < M; column++)
                {
                    _mat[row][column] /= v;
                }
            }
        }
#else
        for (size_t row = 0; row < N; row++)
        {
            for (size_t column = 0; column < M; column++)
            {
                _mat[row][column] /= v;
            }
        }
#endif
        return *this;
    }

    //matrix-vector operators

    template<class TO>
    auto operator*(const vector<TO, M>& vec) const;
public:
    //matrix info and accessors
    static constexpr size_t rows()
    {
        return N;
    }
    static constexpr size_t columns()
    {
        return M;
    }

    T* operator[](size_t x)
    {
        return _mat[x];
    }
    const T* operator[](size_t x) const
    {
        return _mat[x];
    }

    //matrix data is ordered row by row (i.e row|row|row|...|row)
    //same rule applies for iterators

    T* data()
    {
        return _mat.data();
    }
    const T* data() const
    {
        return _mat.data();
    }

    T* begin()
    {
        return _mat.data();
    }
    const T* begin() const
    {
        return _mat.data();
    }

    T* end()
    {
        return _mat.data() + N * M;
    }
    const T* end() const
    {
        return _mat.data() + N * M;
    }
public:
    //comparison operators

    //if two matrices with identical dimensions are compared, they are equal only if every corresponding element is equal
    //and inequal if there is one element that is not equal to corresponding one
    //else matrices are always inequal

    template<class TO, size_t NO, size_t MO>
    bool operator==(const matrix<TO, NO, MO>& other) const
    {
        if constexpr (NO == N && MO == M)
        {
#if USE_OPENMP
            if (is_big_matrix)
            {
#pragma omp parallel for
                for (int row = 0; row < static_cast<int>(N); row++)
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
    
    template<class TO, size_t NO, size_t MO>
    bool operator!=(const matrix<TO, NO, MO>& other) const
    {
        if constexpr (NO == N && MO == M)
        {
#if USE_OPENMP
            if (is_big_matrix)
            {
#pragma omp parallel for
                for (int row = 0; row < static_cast<int>(N); row++)
                {
                    for (size_t column = 0; column < M; column++)
                    {
                        if (!equal(_mat[row][column], other._mat[row][column]))
                        {
                            return true;
                        }
                    }
                }
            }
            else
            {
                for (size_t row = 0; row < N; row++)
                {
                    for (size_t column = 0; column < M; column++)
                    {
                        if (!equal(_mat[row][column], other._mat[row][column]))
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
                    if (!equal(_mat[row][column], other._mat[row][column]))
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
private:
    //helpers

    template<class TO, size_t NO, size_t MO>
    static inline void swap_rows(matrix<TO, NO, MO>& matrix, size_t r1, size_t r2, size_t start_column = 0, size_t end_column = MO)
    {
        for (size_t column = start_column; column < end_column; column++)
        {
            std::swap(matrix._mat[r1][column], matrix._mat[r2][column]);
        }
    }

    template<class TO, size_t NO, size_t MO>
    static inline void swap_columns(matrix<TO, NO, MO>& matrix, size_t c1, size_t c2, size_t start_row = 0, size_t end_row = NO)
    {
        for (size_t row = start_row; row < end_row; row++)
        {
            std::swap(matrix._mat[row][c1], matrix._mat[row][c2]);
        }
    }
public:
    //matrix operations

    template<typename = typename std::enable_if_t<N == M>>
    T determinant() const
    {
        constexpr size_t S = N;

        auto copy = *this;

        //gaussian algorithm is used to turn this matrix into triangular matrix
        //in order to efficently calculate determinant

        T determinant_sign = static_cast<T>(1);
        T determinant_value = static_cast<T>(1);

        for (size_t diagonal = 0; diagonal < S; diagonal++)
        {
            //if current diagonal element is equal to 0 we are trying to find row below diagonal row in which
            //element on current diagonal column is not equal to 0

            if (equal(copy._mat[diagonal][diagonal], static_cast<T>(0)))
            {
                size_t non_zero_row = diagonal;

                for (size_t row = diagonal + 1; row < S; row++)
                {
                    if (!equal(copy._mat[row][diagonal], static_cast<T>(0)))
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

                    determinant_sign *= static_cast<T>(-1);
                }
                else
                {
                    //if non zero row is not found then it implies that matrix determinant is equal to 0
                    return static_cast<T>(0);
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

    template<typename = typename std::enable_if_t<N == M>>
    std::optional<matrix<T, N, M>> inverted() const
    {
        constexpr size_t S = N;

        //trying to convert this matrix to identity matrix
        //by gaussian algorithm to obtain inverse matrix

        matrix<T, S, S * 2> copy(*this);

        //right part of the larger matrix is identity matrix
        //so we need to initialize its diagonal elements to 1
        for (size_t diagonal = 0; diagonal < S; diagonal++)
        {
            copy._mat[diagonal][S + diagonal] = static_cast<T>(1);
        }

        for (size_t diagonal = 0; diagonal < S; diagonal++)
        {
            //if current diagonal element is equal to 0 we are trying to find row below diagonal row in which
            //element on current diagonal column is not equal to 0

            if (equal(copy._mat[diagonal][diagonal], static_cast<T>(0)))
            {
                size_t non_zero_row = diagonal;

                for (size_t row = diagonal + 1; row < S; row++)
                {
                    if (!equal(copy._mat[row][diagonal], static_cast<T>(0)))
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

    size_t rank() const
    {
        auto copy = *this;

        if constexpr(N <= M)
        {
            //there is less or equal rows than column, it means that rank of this matrix can be at max equal to row count

            size_t rank = N;
            for(size_t diagonal = 0; diagonal < rank; diagonal++)
            {
                if(!equal(copy._mat[diagonal][diagonal], static_cast<T>(0)))
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
                        if(!equal(copy._mat[row][diagonal], static_cast<T>(0)))
                        {
                            //row is found, we do not need to reduce matrix rank
                            reduce_rank = false;
                            swap_rows(copy, diagonal, row, diagonal, rank);
                            break;
                        }
                    }

                    if(reduce_rank)
                    {
                        //copying last column to current diagonal column
                        for(size_t row = 0; row < N; row++)
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
                if (!equal(copy._mat[diagonal][diagonal], static_cast<T>(0)))
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
                        if (!equal(copy._mat[diagonal][column], static_cast<T>(0)))
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

    matrix<T, M, N> transposed() const
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
    
    template<typename = typename std::enable_if_t<N == M>>
    matrix<T, M, N>& transpose()
    {
        *this = transposed();
        return *this;
    }
public:
    template<typename = typename std::enable_if_t<N == M>>
    static const matrix<T, N, M>& identity()
    {
        constexpr size_t S = N;

        static matrix<T, S, S>  _identity;
        static bool             _initialized = false;
        if (!_initialized)
        {
            for (size_t diagonal = 0; diagonal < S; diagonal++)
            {
                _identity._mat[diagonal][diagonal] = static_cast<T>(1);
            }
            _initialized = true;
        }
        return _identity;
    }
public:
    template<class TO, size_t NO, size_t MO>
    friend std::ostream& operator<<(std::ostream& os, const matrix<TO, NO, MO>& matrix);

    template<class T, size_t N, size_t M>
    friend equation_system_solution<T, M> solve_equation_system(matrix<T, N, M>&& coefficents, vector<T, N>&& constant_terms);

    template<class T, size_t N, size_t M>
    friend equation_system_solution<T, M> solve_equation_system(const matrix<T, N, M>& coefficents, const vector<T, N>& constant_terms);
};

template<class T, class TO, size_t N, size_t M, size_t P>
auto multiply_matrices(const matrix<TO, N, M>& m1, const matrix<TO, M, P>& m2)
{
    using cm_t = decltype(std::declval<T>() * std::declval<TO>() + std::declval<T>() * std::declval<TO>());

    matrix<cm_t, N, P> result;

#if USE_OPENMP
    if (is_big_matrix)
    {
#pragma omp parallel for
        for (int row = 0; row < static_cast<int>(N); row++)
        {
            for (size_t column = 0; column < P; column++)
            {
                //calculating column-row inner product

                cm_t inner_product = static_cast<cm_t>(0);

                for (size_t k = 0; k < M; k++)
                {
                    inner_product += static_cast<cm_t>(m1[row][k] * m2[k][column]);
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

                cm_t inner_product = static_cast<cm_t>(0);

                for (size_t k = 0; k < M; k++)
                {
                    inner_product += static_cast<cm_t>(m1[row][k] * m2[k][column]);
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

            cm_t inner_product = static_cast<cm_t>(0);

            for (size_t k = 0; k < M; k++)
            {
                inner_product += static_cast<cm_t>(m1[row][k] * m2[k][column]);
            }

            result._mat[row][column] = inner_product;
        }
    }
#endif

    return result;
}

template<class T, template<class...> class TT>
struct is_of_template : std::false_type {};

template<template<class...> class T, class... ARGS>
struct is_of_template<T<ARGS...>, T> : std::true_type {};

template<class T, template<class...> class TT>
constexpr bool is_of_template_v = is_of_template<T, TT>::value;

template<class... MS>
class matrix_multiplication_proxy
{
    //static_assert(is_of_template<MS, matrix>, "Template arguments must be of matrix type");
    template<class... MS>
    friend class matrix_multiplication_proxy;
private:
    std::tuple<MS...> _matrices;

    matrix_multiplication_proxy(std::tuple<MS...>&& ms) :
        _matrices(ms)
    {
    }

    matrix_multiplication_proxy(matrix_multiplication_proxy&& other) = default;

    template<class... MS1, class... MS2, size_t... I1, size_t... I2>
    static std::tuple<MS1..., MS2...> assign(std::tuple<MS1...>&& ms1, std::tuple<MS2...>&& ms2, std::index_sequence<I1...>, std::index_sequence<I2...>)
    {
        return std::make_tuple(std::get<I1>(ms1)..., std::get<I2>(ms2)...);
    }
public:
    matrix_multiplication_proxy()
    {
    }
    matrix_multiplication_proxy(const matrix_multiplication_proxy& other) = delete;

    matrix_multiplication_proxy& operator=(const matrix_multiplication_proxy& other) = delete;
    matrix_multiplication_proxy& operator=(matrix_multiplication_proxy&& other) = delete;

    template<class... MSO>
    matrix_multiplication_proxy<MS..., MSO...> operator*(matrix_multiplication_proxy<MSO...>&& other)
    {
        matrix_multiplication_proxy<MS..., MSO...> result;
        result._matrices = assign(
            std::move(_matrices),
            std::move(other._matrices),
            std::make_index_sequence<sizeof...(MS)>(),
            std::make_index_sequence<sizeof...(MSO)>()
        );
        return result;
    }
private:
    struct matrix_size
    {
        size_t rows;
        size_t columns;
    };

    template<size_t S>
    static constexpr  std::array<std::array<size_t, S>, S> get_multiplication_order(const std::array<matrix_size, S>& sizes)
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

    template<class... TS>
    static constexpr std::array<matrix_size, sizeof...(TS)> get_array()
    {
        return std::array<matrix_size, sizeof...(TS)>({ matrix_size{TS::rows(), TS::columns()}... });
    }

    template<size_t b, size_t e, class... TS, size_t... NS, size_t... MS>
    static auto mult(const std::tuple<matrix<TS, NS, MS>...>& matrices)
    {
        constexpr size_t S = sizeof...(TS);
        constexpr auto arr = get_array<matrix<TS, NS, MS>...>();
        constexpr auto s = get_multiplication_order(arr);

        if constexpr (b == e)
        {
            return std::get<s[b][e]>(matrices);
        }
        if constexpr (s[b][e] > b && s[b][e] <= e)
        {
            return mult<b, s[b][e] - 1>(matrices) * mult<s[b][e], e>(matrices);
        }
    }
public:
    auto operator()() const
    {
        return mult<0, sizeof...(MS) - 1>(_matrices);
    }
};

template<class T, size_t N, size_t M>
template<class... MS, typename>
matrix<T, N, M>::matrix(const matrix_multiplication_proxy<MS...>& proxy)
{
    *this = proxy();
}

template<class T, size_t N, size_t M>
template<class ...MS, typename>
matrix<T, N, M>& matrix<T, N, M>::operator=(const matrix_multiplication_proxy<MS...>& proxy)
{
    *this = proxy();
    return *this;
}

template<class TO, size_t NO, size_t MO>
std::ostream& operator<<(std::ostream& os, const matrix<TO, NO, MO>& matrix)
{
    for (size_t row = 0; row < NO - 1; row++)
    {
        for (size_t column = 0; column < MO - 1; column++)
        {
            os << matrix._mat[row][column] << ",\t";
        }

        os << matrix._mat[row][MO - 1];

        os << std::endl;
    }

    for (size_t column = 0; column < MO - 1; column++)
    {
        os << matrix._mat[NO - 1][column] << ",\t";
    }

    os << matrix._mat[NO - 1][MO - 1];

    return os;
}

template<class T, size_t N, size_t M, class TO, typename = typename std::enable_if_t<std::is_arithmetic_v<TO>>>
inline auto operator*(const TO& v, const matrix<T, N, M>& matrix)
{
    return matrix * v;
}

template<class T, size_t N, size_t M, class TO, typename = typename std::enable_if_t<std::is_arithmetic_v<TO>>>
inline auto operator/(const TO& v, const matrix<T, N, M>& matrix)
{
    return matrix / v;
}

NAMESPACE_LINEAR_ALGEBRA_END