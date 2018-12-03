#pragma once

#include <type_traits>
#include <ostream>
#include <algorithm>
#include <numeric>
#include <initializer_list>

using uint = unsigned int;

template<class T, uint N, uint M>
class matrix;

template<class T, uint D>
class vector_storage_static
{
private:
    T _coords[D];
public:
    inline const T& operator[](size_t d) const
    {
        return _coords[d];
    }
    inline T& operator[](size_t d)
    {
        return _coords[d];
    }
    const T* begin() const
    {
        return _coords;
    }
    T* begin()
    {
        return _coords;
    }
    const T* end() const
    {
        return _coords + D;
    }
    T* end()
    {
        return _coords + D;
    }
};

template<class T, uint D>
class vector_storage_dynamic
{
private:
    T* _coords = nullptr;
public:
    vector_storage_dynamic()
    {
        _mat = new T[D];
    }
    ~vector_storage_dynamic()
    {
        delete[] _mat;
    }
public:
    inline const T& operator[](size_t d) const
    {
        return _coords[d];
    }
    inline T& operator[](size_t d)
    {
        return _coords[d];
    }
    const T* begin() const
    {
        return _coords;
    }
    T* begin()
    {
        return _coords;
    }
    const T* end() const
    {
        return _coords + D;
    }
    T* end()
    {
        return _coords + D;
    }
};

template<class T, uint D>
class vector
{
    static_assert(std::is_arithmetic_v<T>, "Vector element type must be a scalar!");
    template<class T, uint D>
    friend class vector;
    template<class D, uint N0, uint M0>
    friend class matrix;
private:
    using storage_type = typename std::conditional_t<(D >= 10000), vector_storage_dynamic<T, D>, vector_storage_static<T, D>>;
    storage_type _coords;
public:
    vector()
    {
        std::fill(_coords.begin(), _coords.end(), static_cast<T>(0));
    }
    
    template<class T0, uint D0>
    vector(const vector<T0,D0>& other)
    {
        constexpr uint DM = D0 > D ? D : D0;
        std::transform(other._coords.begin(), other._coords.begin() + DM, _coords.begin(), [](const T0& v) {
            return static_cast<T>(v);
        });
        std::fill(_coords.begin() + DM, _coords.begin() + D, static_cast<T>(0));
    }
    template<class T0, uint D0>
    vector(vector<T0, D0>&& other)
    {
        constexpr uint DM = D0 > D ? D : D0;
        std::transform(other._coords.begin(), other._coords.begin() + DM, _coords.begin(), [](const T0& v) {
            return static_cast<T>(v);
        });
        std::fill(_coords.begin() + DM, _coords.begin() + D, static_cast<T>(0));
    }
    
    template<class T0, uint D0>
    vector<T,D>& operator=(const vector<T0, D0>& other)
    {
        constexpr uint DM = D0 > D ? D : D0;
        std::transform(other._coords.begin(), other._coords.begin() + DM, _coords.begin(), [](const T0& v) {
            return static_cast<T>(v);
        });
        return *this;
    }
    template<class T0, uint D0>
    vector<T, D>& operator=(vector<T0, D0>&& other)
    {
        constexpr uint DM = D0 > D ? D : D0;
        std::transform(other._coords.begin(), other._coords.begin() + DM, _coords.begin(), [](const T0& v) {
            return static_cast<T>(v);
        });
        return *this;
    }
    
    template<class T0>
    vector(const T0& v)
    {
        for (uint d = 0; d < D; d++)
        {
            _coords[d] = static_cast<T>(v);
        }
    }
    template<class... T0S>
    vector(const T0S&... vs) :
        vector({ static_cast<T>(vs)... })
    {
    }
    template<class T0>
    vector(std::initializer_list<T0> init_list)
    {
        uint DM = init_list.size() > D ? D : init_list.size();
        std::transform(init_list.begin(), init_list.begin() + DM, _coords.begin(), [](const T0& v) {
            return static_cast<T>(v);
        });
    }

    template<class T0>
    vector<T, D>& operator=(const T0& v)
    {
        for (uint d = 0; d < D; d++)
        {
            _coords[d] = static_cast<T>(v);
        }
        return *this;
    }
    template<class... T0S>
    vector<T, D>& operator=(const T0S&... vs)
    {
        return operator=({ static_cast<T>(vs)... });
    }
    template<class T0>
    vector<T, D>& operator=(std::initializer_list<T0> init_list)
    {
        uint DM = init_list.size() > D ? D : static_cast<uint>(init_list.size());
        std::transform(init_list.begin(), init_list.begin() + DM, _coords.begin(), [](const T0& v) {
            return static_cast<T>(v);
        });
        return *this;
    }
public:
    template<class T0, uint D0>
    auto operator+(const vector<T0, D0>& other) const
    {
        constexpr uint DM = D0 > D ? D : D0;
        using cm_t = decltype(std::declval<T>() + std::declval<T0>());
        vector<cm_t, DM> result(*this);
        for (uint d = 0; d < DM; d++)
        {
            result._coords[d] += other._coords[d];
        }
        return result;
    }
    template<class T0, uint D0>
    auto operator-(const vector<T0, D0>& other) const
    {
        constexpr uint DM = D0 > D ? D : D0;
        using cm_t = decltype(std::declval<T>() + std::declval<T0>());
        vector<cm_t, DM> result(*this);
        for (uint d = 0; d < DM; d++)
        {
            result._coords[d] -= other._coords[d];
        }
        return result;
    }
    template<class T0>
    auto operator*(const T0& v) const
    {
        using cm_t = decltype(std::declval<T>() * std::declval<T0>());
        vector<cm_t, D> result(*this);
        std::transform(result._coords.begin(), result._coords.end(), result._coords.begin(), [&v](const T& vec_dim) {
            return vec_dim * v;
        });
        return result;
    }
    template<class T0>
    auto operator/(const T0& v) const
    {
        using cm_t = decltype(std::declval<T>() * std::declval<T0>());
        vector<cm_t, D> result(*this);
        std::transform(result._coords.begin(), result._coords.end(), result._coords.begin(), [&v](const T& vec_dim) {
            return vec_dim / v;
        });
        return result;
    }

    template<class T0, uint D0>
    vector<T, D>& operator+=(const vector<T0, D0>& other)
    {
        constexpr uint DM = D0 > D ? D : D0;
        for (uint d = 0; d < DM; d++)
        {
            _coords[d] += static_cast<T>(other._coords[d]);
        }
        return *this;
    }
    template<class T0, uint D0>
    vector<T, D>& operator-=(const vector<T0, D0>& other)
    {
        constexpr uint DM = D0 > D ? D : D0;
        for (uint d = 0; d < DM; d++)
        {
            _coords[d] -= static_cast<T>(other._coords[d]);
        }
        return *this;
    }
    template<class T0>
    vector<T, D>& operator*=(const T0& v)
    {
        std::transform(_coords.begin(), _coords.end(), _coords.begin(), [&v](const T& vec_dim) {
            return vec_dim * v;
        });
        return *this;
    }
    template<class T0>
    vector<T, D>& operator/=(const T0& v)
    {
        std::transform(_coords.begin(), _coords.end(), _coords.begin(), [&v](const T& vec_dim) {
            return vec_dim / v;
        });
        return *this;
    }
    
    constexpr uint dimension() const
    {
        return D;
    }
    T& operator[](uint d)
    {
        return _coords[d];
    }
    const T& operator[](uint d) const
    {
        return _coords[d];
    }
    T* data() 
    {
        return _coords;
    }
    const T* data() const
    {
        return _coords;
    }

    T* begin()
    {
        return _coords.begin();
    }
    const T* begin() const
    {
        return _coords.begin();
    }
    T* end()
    {
        return _coords.end();
    }
    const T* end() const
    {
        return _coords.end();
    }

    template<class D0, uint M>
    auto operator*(const matrix<D0, D, M>& matrix) const;
    template<class D0>
    vector<T, D>& operator*=(const matrix<D0, D, D>& matrix) const;

    bool operator==(const vector<T, D>& other) const
    {
        for (uint d = 0; d < D; d++)
        {
            if(std::abs(other._coords[d] - _coords[d]) > std::numeric_limits<T>::epsilon())
            {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const vector<T, D>& other) const
    {
        for (uint d = 0; d < D; d++)
        {
            if (std::abs(other._coords[d] - _coords[d]) > std::numeric_limits<T>::epsilon())
            {
                return true;
            }
        }
        return false;
    }
public:
    template<typename = typename std::enable_if_t<D >=1 >>
    T& x()
    {
        return _coords[0];
    }
    template<typename = typename std::enable_if_t<D >= 1>>
    const T& x() const
    {
        return _coords[0];
    }
    template<typename = typename std::enable_if_t<D >= 1>>
    void set_x(const T& x)
    {
        _coords[0] = x;
    }
    
    template<typename = typename std::enable_if_t<D >= 2>>
    T& y()
    {
        return _coords[1];
    }
    template<typename = typename std::enable_if_t<D >= 2>>
    const T& y() const
    {
        return _coords[1];
    }
    template<typename = typename std::enable_if_t<D >= 2>>
    void set_y(const T& y)
    {
        _coords[1] = y;
    }
    
    template<typename = typename std::enable_if_t<D >= 3>>
    T& z()
    {
        return _coords[2];
    }
    template<typename = typename std::enable_if_t<D >= 3>>
    const T& z() const
    {
        return _coords[2];
    }
    template<typename = typename std::enable_if_t<D >= 3>>
    void set_z(const T& z)
    {
        _coords[2] = z;
    }
    
    template<typename = typename std::enable_if_t<D >= 4>>
    T& w()
    {
        return _coords[3];
    }
    template<typename = typename std::enable_if_t<D >= 4>>
    const T& w() const
    {
        return _coords[3];
    }
    template<typename = typename std::enable_if_t<D >= 4>>
    void set_w(const T& w)
    {
        _coords[3] = w;
    }

    template<typename = typename std::enable_if_t<D == 2>>
    vector<T, D> perpendicularLH() const
    {
        return { _coords[1], -_coords[0] };
    }
    template<typename = typename std::enable_if_t<D == 2>>
    vector<T, D> perpendicularRH() const
    {
        return { -_coords[1], _coords[0] };
    }

    template<typename = typename std::enable_if_t<D == 3>>
    vector<T, D> perpendicularLH_linear() const
    {
        return { _coords[1], -_coords[0] };
    }
    template<typename = typename std::enable_if_t<D == 3>>
    vector<T, D> perpendicularRH_linear() const
    {
        return { -_coords[1], _coords[0] };
    }

    template<class T0, typename = typename std::enable_if_t<D == 3>>
    vector<T, D> cross_productLH(const vector<T0, D>& other) const
    {
        return {
            _coords[2] * static_cast<T>(other._coords[1]) - _coords[1] * static_cast<T>(other._coords[2]),
            _coords[2] * static_cast<T>(other._coords[0]) - _coords[0] * static_cast<T>(other._coords[2]),
            _coords[1] * static_cast<T>(other._coords[0]) -_coords[0] * static_cast<T>(other._coords[1])
        };
    }
    template<class T0, typename = typename std::enable_if_t<D == 3>>
    vector<T, D> cross_productRH(const vector<T0, D>& other) const
    {
        return {
            _coords[1] * static_cast<T>(other._coords[2]) - _coords[2] * static_cast<T>(other._coords[1]),
            _coords[0] * static_cast<T>(other._coords[2]) - _coords[2] * static_cast<T>(other._coords[0]),
            _coords[0] * static_cast<T>(other._coords[1]) - _coords[1] * static_cast<T>(other._coords[0])
        };
    }

    template<class T0, typename = typename std::enable_if_t<D == 4>>
    vector<T, D> cross_productLH_linear(const vector<T0, D>& other) const
    {
        return {
            _coords[2] * static_cast<T>(other._coords[1]) - _coords[1] * static_cast<T>(other._coords[2]),
            _coords[2] * static_cast<T>(other._coords[0]) - _coords[0] * static_cast<T>(other._coords[2]),
            _coords[1] * static_cast<T>(other._coords[0]) - _coords[0] * static_cast<T>(other._coords[1])
        };
    }
    template<class T0, typename = typename std::enable_if_t<D == 4>>
    vector<T, D> cross_productRH_linear(const vector<T0, D>& other) const
    {
        return {
            _coords[1] * static_cast<T>(other._coords[2]) - _coords[2] * static_cast<T>(other._coords[1]),
            _coords[0] * static_cast<T>(other._coords[2]) - _coords[2] * static_cast<T>(other._coords[0]),
            _coords[0] * static_cast<T>(other._coords[1]) - _coords[1] * static_cast<T>(other._coords[0])
        };
    }

    inline auto magnitude() const
    {
        auto ip = std::inner_product(_coords.begin(), _coords.end(), _coordsbegin(), static_cast<T>(0));
        return std::sqrt(ip);
    }
    inline auto magnitude_sqr() const
    {
        return std::inner_product(_coords.begin(), _coords.end(), _coords.begin(), static_cast<T>(0));
    }
    inline auto length() const
    {
        return magnitude();
    }
    inline auto length_sqr() const
    {
        return magnitude_sqr();
    }
    inline auto normalized() const
    {
        return *this / length();
    }
    inline vector<T, D>& normalize()
    {
        *this /= length();
        return *this;
    }
    template<class T0>
    inline auto inner_product(const vector<T0,D>& other)
    {
        using cm_t = decltype(std::declval<T0>() * std::declval<T>() + std::declval<T0>() * std::declval<T>());
        return std::inner_product(_coords.begin(), _coords.end(), other._coords.begin(), static_cast<cm_t>(0));
    }
public:
    template<class T0, uint D0>
    friend std::ostream& operator<<(std::ostream& os, const vector<T0, D0>& vector);
public:
    static const vector<T, D>& one()
    {
        static vector <T, D> _one = static_cast<T>(1);
        return _one;
    }
    static const vector<T, D>& zero()
    {
        static vector <T, D> _zero;
        return _zero;
    }
    static const vector<T, D>& linear_zero() 
    {
        static vector <T, D> _zero;
        static bool _initialized = false;
        if(!_initialized)
        {
            _zero._coords[D - 1] = static_cast<T>(1);
            _initialized = true;
        }
        return _zero;
    }
    static const vector<T, D>& linear_one()
    {
        static vector <T, D> _one = static_cast<T>(1);
        return _one;
    }
};

template<class T0, uint D0>
std::ostream& operator<<(std::ostream& os, const vector<T0, D0>& vector)
{
    for (size_t d = 0; d < D0; d++)
    {
        os << vector._coords[d] << ", ";
    }
    return os;
}