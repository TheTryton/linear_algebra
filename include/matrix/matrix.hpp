#pragma once

#include <type_traits>
#include <ostream>
#include <initializer_list>
#include <optional>

using uint = unsigned int;

template<class T, uint D>
class vector;

template<class T, uint N, uint M>
class matrix
{
    static_assert(std::is_arithmetic_v<T>, "Matrix element type must be a scalar!");
    template<class D, uint N0, uint M0>
    friend class matrix;
    template<class T, uint D>
    friend class vector;
private:
    T _mat[N][M];
private:
    class matrix_proxy_column
    {
    private:
        uint _x;
        matrix<T, N, M>& _ref;
    public:
        matrix_proxy_column() = delete;
        matrix_proxy_column(matrix<T, N, M>& ref, uint x):
            _ref(ref),
            _x(x)
        {
        }
        matrix_proxy_column(const matrix_proxy_column& other):
            _ref(other._ref),
            _x(other._x)
        {
        }
        matrix_proxy_column(matrix_proxy_column&& other) :
            _ref(other._ref),
            _x(other._x)
        {
        }
        matrix_proxy_column& operator=(const matrix_proxy_column& other)
        {
            _ref = other._ref;
            _x = other._x;
            return *this;
        }
        matrix_proxy_column& operator=(matrix_proxy_column&& other)
        {
            _ref = other._ref;
            _x = other._x;
            return *this;
        }
        bool operator==(const matrix_proxy_column& other) const
        {
            return (&other._ref == &_ref) && (_x == other._x);
        }
        bool operator!=(const matrix_proxy_column& other) const
        {
            return (&other._ref != &_ref) || (_x != other._x);
        }
    public:
        T& operator[](uint y)
        {
            return _ref._mat[_x][y];
        }
        const T& operator[](uint y) const
        {
            return _ref._mat[_x][y];
        }
    };
    class const_matrix_proxy_column
    {
    private:
        uint _x;
        const matrix<T, N, M>& _ref;
    public:
        const_matrix_proxy_column() = delete;
        const_matrix_proxy_column(const matrix<T, N, M>& ref, uint x) :
            _ref(ref),
            _x(x)
        {
        }
        const_matrix_proxy_column(const const_matrix_proxy_column& other) :
            _ref(other._ref),
            _x(other._x)
        {
        }
        const_matrix_proxy_column(const_matrix_proxy_column&& other) :
            _ref(other._ref),
            _x(other._x)
        {
        }
        const_matrix_proxy_column& operator=(const const_matrix_proxy_column& other)
        {
            _ref = other._ref;
            _x = other._x;
            return *this;
        }
        const_matrix_proxy_column& operator=(const_matrix_proxy_column&& other)
        {
            _ref = other._ref;
            _x = other._x;
            return *this;
        }
        bool operator==(const const_matrix_proxy_column& other) const
        {
            return (&other._ref == &_ref) && (_x == other._x);
        }
        bool operator!=(const const_matrix_proxy_column& other) const
        {
            return (&other._ref != &_ref) || (_x != other._x);
        }
    public:
        const T& operator[](uint y) const
        {
            return _ref._mat[_x][y];
        }
    };
    class matrix_proxy_row
    {
    private:
        uint _y;
        matrix<T, N, M>& _ref;
    public:
        matrix_proxy_row() = delete;
        matrix_proxy_row(matrix<T, N, M>& ref, uint y) :
            _ref(ref),
            _y(y)
        {
        }
        matrix_proxy_row(const matrix_proxy_column& other) :
            _ref(other._ref),
            _y(other._y)
        {
        }
        matrix_proxy_row(matrix_proxy_column&& other) :
            _ref(other._ref),
            _y(other._y)
        {
        }
        matrix_proxy_row& operator=(const matrix_proxy_row& other)
        {
            _ref = other._ref;
            _y = other._y;
            return *this;
        }
        matrix_proxy_row& operator=(matrix_proxy_row&& other)
        {
            _ref = other._ref;
            _y = other._y;
            return *this;
        }
        bool operator==(const matrix_proxy_row& other) const
        {
            return (&other._ref == &_ref) && (_y == other._y);
        }
        bool operator!=(const matrix_proxy_row& other) const
        {
            return (&other._ref != &_ref) || (_y != other._y);
        }
    public:
        T& operator[](uint x)
        {
            return _ref._mat[x][_y];
        }
        const T& operator[](uint x) const
        {
            return _ref._mat[x][_y];
        }
    };
    class const_matrix_proxy_row
    {
    private:
        uint _y;
        const matrix<T, N, M>& _ref;
    public:
        const_matrix_proxy_row() = delete;
        const_matrix_proxy_row(const matrix<T, N, M>& ref, uint y) :
            _ref(ref),
            _y(y)
        {
        }
        const_matrix_proxy_row(const const_matrix_proxy_row& other) :
            _ref(other._ref),
            _y(other._y)
        {
        }
        const_matrix_proxy_row(const_matrix_proxy_row&& other) :
            _ref(other._ref),
            _y(other._y)
        {
        }
        const_matrix_proxy_row& operator=(const const_matrix_proxy_row& other)
        {
            _ref = other._ref;
            _y = other._y;
            return *this;
        }
        const_matrix_proxy_row& operator=(const_matrix_proxy_row&& other)
        {
            _ref = other._ref;
            _y = other._y;
            return *this;
        }
        bool operator==(const const_matrix_proxy_row& other) const
        {
            return (&other._ref == &_ref) && (_y == other._y);
        }
        bool operator!=(const const_matrix_proxy_row& other) const
        {
            return (&other._ref != &_ref) || (_y != other._y);
        }
    public:
        const T& operator[](uint x) const
        {
            return _ref._mat[x][_y];
        }
    };
public:
    matrix()
    {
        for (uint x = 0; x < N; x++)
        {
            for (uint y = 0; y < M; y++)
            {
                _mat[x][y] = static_cast<T>(0);
            }
        }
    }

    template<class D, uint N0, uint M0>
    matrix(const matrix<D, N0, M0>& other)
    {
        constexpr uint NN = N0 > N ? N : N0;
        constexpr uint MM = M0 > M ? M : M0;
        for (uint x = 0; x < NN; x++)
        {
            for (uint y = 0; y < MM; y++)
            {
                _mat[x][y] = static_cast<T>(other._mat[x][y]);
            }
        }
        for (uint x = NN; x < N; x++)
        {
            for (uint y = 0; y < M; y++)
            {
                _mat[x][y] = static_cast<T>(0);
            }
        }
        for (uint x = 0; x < N; x++)
        {
            for (uint y = MM; y < M; y++)
            {
                _mat[x][y] = static_cast<T>(0);
            }
        }
    }
    template<class D, uint N0, uint M0>
    matrix(matrix<D, N0, M0>&& other)
    {
        constexpr uint NN = N0 > N ? N : N0;
        constexpr uint MM = M0 > M ? M : M0;
        for (uint x = 0; x < NN; x++)
        {
            for (uint y = 0; y < MM; y++)
            {
                _mat[x][y] = other._mat[x][y];
            }
        }
        for (uint x = NN; x < N; x++)
        {
            for (uint y = 0; y < M; y++)
            {
                _mat[x][y] = static_cast<T>(0);
            }
        }
        for (uint x = 0; x < N; x++)
        {
            for (uint y = MM; y < M; y++)
            {
                _mat[x][y] = static_cast<T>(0);
            }
        }
    }
    
    template<class D, uint N0, uint M0>
    matrix<T, N, M>& operator=(const matrix<D, N0, M0>& other)
    {
        constexpr uint NN = N0 > N ? N : N0;
        constexpr uint MM = M0 > M ? M : M0;
        for (uint x = 0; x < NN; x++)
        {
            for (uint y = 0; y < MM; y++)
            {
                _mat[x][y] = other._mat[x][y];
            }
        }
    }
    template<class D, uint N0, uint M0>
    matrix<T, N, M>& operator=(matrix<D, N0, M0>&& other)
    {
        constexpr uint NN = N0 > N ? N : N0;
        constexpr uint MM = M0 > M ? M : M0;
        for (uint x = 0; x < NN; x++)
        {
            for (uint y = 0; y < MM; y++)
            {
                _mat[x][y] = other._mat[x][y];
            }
        }
    }

    template<class D>
    matrix(const D& v)
    {
        for (uint x = 0; x < N; x++)
        {
            for (uint y = 0; y < M; y++)
            {
                _mat[x][y] = static_cast<T>(v);
            }
        }
    }
    template<class... DS>
    matrix(const DS&... vs):
        matrix({ static_cast<T>(vs)... })
    {
    }
    template<class D>
    matrix(std::initializer_list<D> vs)
    {
        auto b = vs.begin();
        for (uint y = 0; y < M; y++)
        {
            for (uint x = 0; x < N; x++)
            {
                if (b == vs.end())
                {
                    _mat[x][y] = static_cast<T>(0);
                }
                else
                {
                    _mat[x][y] = static_cast<T>(*(b++));
                }
            }
        }
    }
    template<class D>
    matrix(std::initializer_list<std::initializer_list<D>> vs)
    {
        auto by = vs.begin();
        for (uint y = 0; y < M; y++)
        {
            if(by == vs.end())
            {
                for (uint x = 0; x < N; x++)
                {
                    _mat[x][y] = static_cast<T>(0);
                }
            }
            else
            {
                auto bx = (*by).begin();
                auto ex = (*(by++)).end();
                for (uint x = 0; x < N; x++)
                {
                    if (bx == ex)
                    {
                        _mat[x][y] = static_cast<T>(0);
                    }
                    else
                    {
                        _mat[x][y] = static_cast<T>(*(bx++));
                    }
                }
            }
        }
    }

    template<class D>
    matrix<T, N, M>& operator=(const D& v)
    {
        for (uint x = 0; x < N; x++)
        {
            for (uint y = 0; y < M; y++)
            {
                _mat[x][y] = static_cast<T>(v);
            }
        }
        return *this;
    }
    template<class... DS>
    matrix<T, N, M>& operator=(const DS&... vs)
    {
        return operator=({ static_cast<T>(vs)... });
    }
    template<class D>
    matrix<T, N, M>& operator=(std::initializer_list<D> vs)
    {
        auto b = vs.begin();
        for (uint y = 0; y < M; y++)
        {
            for (uint x = 0; x < N; x++)
            {
                if (b == vs.end())
                {
                    return;
                }
                _mat[x][y] = static_cast<T>(*(b++));
            }
        }
        return *this;
    }
    template<class D>
    matrix<T, N, M>& operator=(std::initializer_list<std::initializer_list<D>> vs)
    {
        auto by = vs.begin();
        for (uint y = 0; y < M; y++)
        {
            if (by == vs.end())
            {
                return;
            }
            auto bx = (*by).begin();
            auto ex = (*(by++)).end();
            for (uint x = 0; x < N; x++)
            {
                if (bx == ex)
                {
                    break;
                }
                _mat[x][y] = static_cast<T>(*(bx++));
            }
        }
        return *this;
    }
public:
    //matrix-matrix operators
    template<class D, uint N0, uint M0>
    auto operator+(const matrix<D, N0, M0>& other) const
    {
        constexpr uint NN = N0 > N ? N0 : N;
        constexpr uint MM = M0 > M ? M0 : M;
        using mt_t = decltype(std::declval<T>() + std::declval<D>());
        matrix<mt_t, NN, MM> result(*this);
        for (uint x = 0; x < N0; x++)
        {
            for (uint y = 0; y < M0; y++)
            {
                result._mat[x][y] += other._mat[x][y];
            }
        }
        return result;
    }
    template<class D, uint N0, uint M0>
    matrix<T,N,M>& operator+=(const matrix<D, N0, M0>& other)
    {
        constexpr uint NN = N0 > N ? N0 : N;
        constexpr uint MM = M0 > M ? M0 : M;
        for (uint x = 0; x < N0; x++)
        {
            for (uint y = 0; y < M0; y++)
            {
                _mat[x][y] += static_cast<T>(other._mat[x][y]);
            }
        }
        return *this;
    }
    template<class D, uint N0, uint M0>
    auto operator-(const matrix<D, N0, M0>& other) const
    {
        constexpr uint NN = N0 > N ? N0 : N;
        constexpr uint MM = M0 > M ? M0 : M;
        using mt_t = decltype(std::declval<T>() + std::declval<D>());
        matrix<mt_t, NN, MM> result(*this);
        for (uint x = 0; x < N0; x++)
        {
            for (uint y = 0; y < M0; y++)
            {
                result._mat[x][y] -= other._mat[x][y];
            }
        }
        return result;
    }
    template<class D, uint N0, uint M0>
    matrix<T, N, M>& operator-=(const matrix<D, N0, M0>& other)
    {
        constexpr uint NN = N0 > N ? N0 : N;
        constexpr uint MM = M0 > M ? M0 : M;
        for (uint x = 0; x < N0; x++)
        {
            for (uint y = 0; y < M0; y++)
            {
                _mat[x][y] -= static_cast<T>(other._mat[x][y]);
            }
        }
        return *this;
    }
    template<class D, uint P>
    auto operator*(const matrix<D, M, P>& other) const
    {
        using mt_t = decltype(std::declval<T>() * std::declval<D>() + std::declval<T>() * std::declval<D>());
        matrix<mt_t, N, P> result;
        for (uint x = 0; x < N; x++)
        {
            for (uint y = 0; y < P; y++)
            {
                mt_t inner_product = static_cast<mt_t>(0);
                for (uint k = 0; k < M; k++)
                {
                    inner_product += static_cast<mt_t>(_mat[x][k] * other._mat[k][y]);
                }
                result._mat[x][y] = inner_product;
            }
        }
        return result;
    }
    template<class D, uint P, typename = typename std::enable_if_t<M==P>>
    matrix<T,N,M>& operator*=(const matrix<D, M, P>& other)
    {
        *this = operator*(other);
        return *this;
    }
    
    //matrix-scalar operators
    matrix<T, N, M> operator-() const
    {
        matrix<T, N, M> result(*this);
        for (uint x = 0; x < N; x++)
        {
            for (uint y = 0; y < M; y++)
            {
                result._mat[x][y] *= static_cast<T>(-1);
            }
        }
        return result;
    }
    template<class D, typename = typename std::enable_if_t<std::is_arithmetic_v<D>>>
    auto operator*(const D& v) const
    {
        using mt_t = decltype(std::declval<T>()*std::declval<D>());
        matrix<mt_t, N, M> result(*this);
        for (uint x = 0; x < N; x++)
        {
            for (uint y = 0; y < M; y++)
            {
                result._mat[x][y] *= v;
            }
        }
        return result;
    }
    template<class D, typename = typename std::enable_if_t<std::is_arithmetic_v<D>>>
    matrix<T, N, M>& operator*=(const D& v)
    {
        for (uint x = 0; x < N; x++)
        {
            for (uint y = 0; y < M; y++)
            {
                _mat[x][y] *= static_cast<T>(v);
            }
        }
        return *this;
    }
    template<class D, typename = typename std::enable_if_t<std::is_arithmetic_v<D>>>
    auto operator/(const D& v) const
    {
        using mt_t = decltype(std::declval<T>()/std::declval<D>());
        matrix<mt_t, N, M> result(*this);
        for (uint x = 0; x < N; x++)
        {
            for (uint y = 0; y < M; y++)
            {
                result._mat[x][y] /= v;
            }
        }
        return result;
    }
    template<class D, typename = typename std::enable_if_t<std::is_arithmetic_v<D>>>
    matrix<T, N, M>&  operator/=(const D& v)
    {
        for (uint x = 0; x < N; x++)
        {
            for (uint y = 0; y < M; y++)
            {
                _mat[x][y] /= static_cast<T>(v);
            }
        }
        return *this;
    }

    //matrix-vector operators
    template<class D0>
    auto operator*(const vector<D0, N>& vector) const;

    constexpr uint rows() const
    {
        return N;
    }
    constexpr uint columns() const
    {
        return M;
    }
    matrix_proxy_column operator[](uint x)
    {
        return matrix_proxy_column(*this, x);
    }
    const_matrix_proxy_column operator[](uint x) const
    {
        return const_matrix_proxy_column(*this, x);
    }
    matrix_proxy_column column(uint x)
    {
        return matrix_proxy_column(*this, x);
    }
    const_matrix_proxy_column column(uint x) const
    {
        return const_matrix_proxy_column(*this, x);
    }
    matrix_proxy_row row(uint y)
    {
        return matrix_proxy_column(*this, y);
    }
    const_matrix_proxy_row row(uint y) const
    {
        return const_matrix_proxy_column(*this, y);
    }

    bool operator==(const matrix<T, N, M>& other) const
    {
        for (uint x = 0; x < N; x++)
        {
            for (uint y = 0; y < M; y++)
            {
                if(std::abs(other._mat[x][y] - _mat[x][y]) > std::numeric_limits<T>::epsilon())
                {
                    return false;
                }
            }
        }
        return true;
    }
    bool operator!=(const matrix<T, N, M>& other) const
    {
        for (uint x = 0; x < N; x++)
        {
            for (uint y = 0; y < M; y++)
            {
                if (std::abs(other._mat[x][y] - _mat[x][y]) > std::numeric_limits<T>::epsilon())
                {
                    return true;
                }
            }
        }
        return false;
    }
public:
    template<typename = typename std::enable_if_t<N == M>>
    std::optional<T> determinant() const
    {
        matrix<T, N, M> copy(*this);
        //gaussian algorithm to turn matrix into triangular matrix
        T det_sign = static_cast<T>(1);
        T det = static_cast<T>(1);
        for (uint xy = 0; xy < N; xy++)
        {
            if(std::abs(copy._mat[xy][xy]) <= std::numeric_limits<T>::epsilon())
            {
                uint non_zero_row = xy;
                for (uint y = xy + 1; y < M; y++)
                {
                    if(std::abs(copy._mat[xy][y]) > std::numeric_limits<T>::epsilon())
                    {
                        non_zero_row = y;
                        break;
                    }
                }
                if(non_zero_row != xy)
                {
                    det_sign *= static_cast<T>(-1);
                    for (uint x = 0; x < N; x++)
                    {
                        std::swap(copy._mat[x][xy], copy._mat[x][non_zero_row]);
                    }
                }
                else
                {
                    return std::optional<T>();
                }
            }
            for (uint y = xy + 1; y < M; y++)
            {
                T factor = copy._mat[xy][y] / copy._mat[xy][xy];
                for(uint x = xy; x < N; x++)
                {
                    copy._mat[x][y] -= copy._mat[x][xy] * factor;
                }
            }
            det *= copy._mat[xy][xy];
        }
        return det * det_sign;
    }
    template<typename = typename std::enable_if_t<N == M && std::is_floating_point_v<T>>>
    std::optional<matrix<T, N, M>> inversed() const
    {
        matrix<T, N * 2, M> copy(*this);
        for (uint xy = 0; xy < N; xy++)
        {
            copy._mat[N + xy][xy] = static_cast<T>(1);
        }
        
        for (uint xy = 0; xy < N; xy++)
        {
            if (std::abs(copy._mat[xy][xy]) <= std::numeric_limits<T>::epsilon())
            {
                uint non_zero_row = xy;
                for (uint y = xy + 1; y < M; y++)
                {
                    if (std::abs(copy._mat[xy][y]) > std::numeric_limits<T>::epsilon())
                    {
                        non_zero_row = y;
                        break;
                    }
                }
                if (non_zero_row != xy)
                {
                    for (uint x = 0; x < N * 2; x++)
                    {
                        std::swap(copy._mat[x][xy], copy._mat[x][non_zero_row]);
                    }
                }
                else
                {
                    return std::optional<matrix<T, N, M>>();
                }
            }

            T factor = copy._mat[xy][xy];
            for (uint x = xy; x < N * 2; x++)
            {
                copy._mat[x][xy] /= factor;
            }

            for (uint y = xy + 1; y < M; y++)
            {
                T factor = copy._mat[xy][y];
                for (uint x = xy; x < N * 2; x++)
                {
                    copy._mat[x][y] -= copy._mat[x][xy] * factor;
                }
            }
        }

        for(uint xy = 0; xy < N; xy++)
        {
            for (uint y = 0; y < xy; y++)
            {
                T factor = copy._mat[xy][y];
                for (uint x = xy; x < N * 2; x++)
                {
                    copy._mat[x][y] -= copy._mat[x][xy] * factor;
                }
            }
        }

        matrix<T, N, M> result;
        for (uint x = 0; x < N; x++)
        {
            for (uint y = 0; y < M; y++)
            {
                result._mat[x][y] = copy._mat[N + x][y];
            }
        }
        return result;
    }
    matrix<T, M, N> transposed() const
    {
        matrix<T, M, N> result;
        for (size_t x = 0; x < N; x++)
        {
            for (size_t y = 0; y < M; y++)
            {
                result._mat[y][x] = _mat[x][y];
            }
        }
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
        static matrix<T, N, M>  _identity;
        static bool             _initialized = false;
        if(!_initialized)
        {
            for(uint xy = 0; xy < N; xy++)
            {
                _identity._mat[xy][xy] = static_cast<T>(1);
            }
            _initialized = true;
        }
        return _identity;
    }
public:
    template<class D, uint N0, uint M0>
    friend std::ostream& operator<<(std::ostream& os, const matrix<D, N0, M0>& matrix);
};

template<class D, uint N0, uint M0>
std::ostream& operator<<(std::ostream& os, const matrix<D, N0, M0>& matrix)
{
    for (size_t y = 0; y < M0; y++)
    {
        for (size_t x = 0; x < N0;x++)
        {
            os << matrix._mat[x][y] << ",\t";
        }
        os << std::endl;
    }
    return os;
}