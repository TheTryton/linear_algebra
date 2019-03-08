#pragma once

#include "../linear_algebra_common.hpp"

NAMESPACE_LINEAR_ALGEBRA_BEGIN

template<class T, size_t D>
class vector
{
    static_assert(D != 0 , "Vector dimension must be at least 1!");
    static_assert(std::is_arithmetic_v<T>, "Vector element type must be a scalar!");
    static_assert(std::is_default_constructible_v<T>, "Vector element type must be default constructible!");
    template<class T, size_t D>
    friend class vector;
    template<class D, size_t N0, size_t M0>
    friend class matrix;
private:
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

    class vector_storage_dynamic
    {
    private:
        T* _coords = nullptr;
    public:
        vector_storage_dynamic()
        {
            _coords = new T[D];
        }
        vector_storage_dynamic(vector_storage_dynamic&& other)
        {
            _coords = other._coords;
            other._coords = nullptr;
        }
        vector_storage_dynamic& operator=(vector_storage_dynamic&& other)
        {
            _coords = other._coords;
            other._coords = nullptr;
            return *this;
        }
        ~vector_storage_dynamic()
        {
            if (_coords)
            {
                delete[] _coords;
            }
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

    using storage_type = typename std::conditional_t<(D * sizeof(T) >= 80000), vector_storage_dynamic, vector_storage_static>;
    storage_type _coords;
public:
    using calculation_type = T;
public:
    //default constructor

    vector()
    {
        //filling vector to initialize it with 0-oes
        std::fill(_coords.begin(), _coords.end(), static_cast<T>(0));
    }
    
    //copy, move constructors and operators

    //same type, same dimensions

    vector(const vector<T, D>& other)
    {
        /*
            filling vector D with vector D
            vector D elements are copied to
            elements in vector D
        */

        std::copy(other._coords.begin(), other._coords.end(), _coords.begin());
    }

    vector(vector<T, D>&& other)
    {
        /*
            filling vector D with vector D
            vector D elements are copied to
            elements in vector D

            move optimization for dynamic storage
        */

        if constexpr (std::is_same_v<storage_type, vector_storage_dynamic>)
        {
            _coords = std::move(other._coords);
        }
        if constexpr (!std::is_same_v<storage_type, vector_storage_dynamic>)
        {
            std::copy(other._coords.begin(), other._coords.end(), _coords.begin());
        }
    }

    vector<T, D>& operator=(const vector<T, D>& other)
    {
        /*
            filling vector D with vector D
            vector D elements are copied to
            elements in vector D
        */

        std::copy(other._coords.begin(), other._coords.end(), _coords.begin());

        return *this;
    }
    
    vector<T, D>& operator=(vector<T, D>&& other)
    {
        /*
            filling vector D with vector D
            vector D elements are copied to
            elements in vector D

            move optimization for dynamic storage
        */

        if constexpr (std::is_same_v<storage_type, vector_storage_dynamic>)
        {
            _coords = std::move(other._coords);
        }
        if constexpr (!std::is_same_v<storage_type, vector_storage_dynamic>)
        {
            std::copy(other._coords.begin(), other._coords.end(), _coords.begin());
        }

        return *this;
    }

    //different type, same dimensions

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector(const vector<TO, D>& other)
    {
        /*
            filling vector D with vector D
            vector D elements are copied to
            elements in vector D
        */

        std::transform(other._coords.begin(), other._coords.end(), _coords.begin(), [](const TO& v) {
            return static_cast<T>(v);
        });
    }

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector(vector<TO, D>&& other)
    {
        /*
            filling vector D with vector D
            vector D elements are copied to
            elements in vector D
        */

        std::transform(other._coords.begin(), other._coords.end(), _coords.begin(), [](const TO& v) {
            return static_cast<T>(v);
        });
    }
    
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator=(const vector<TO, D>& other)
    {
        /*
            filling vector D with vector D
            vector D elements are copied to
            elements in vector D
        */

        std::transform(other._coords.begin(), other._coords.end(), _coords.begin(), [](const TO& v) {
            return static_cast<T>(v);
        });

        return *this;
    }

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator=(vector<TO, D>&& other)
    {
        /*
            filling vector D with vector D
            vector D elements are copied to
            elements in vector D
        */

        std::transform(other._coords.begin(), other._coords.end(), _coords.begin(), [](const TO& v) {
            return static_cast<T>(v);
        });

        return *this;
    }

    //different type, different dimensions

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector(const vector<TO, DO>& other)
    {
        /*
            filling vector D with vector DO
            vector DO elements are copied to
            elements in vector D, other 
            elements are set to 0
        */

        constexpr size_t DM = DO > D ? D : DO;

        std::transform(other._coords.begin(), other._coords.begin() + DM, _coords.begin(), [](const TO& v) {
            return static_cast<T>(v);
        });

        std::fill(_coords.begin() + DM, _coords.end(), static_cast<T>(0));
    }

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector(vector<TO, DO>&& other)
    {
        /*
            filling vector D with vector DO
            vector DO elements are copied to
            elements in vector D, other
            elements are set to 0
        */

        constexpr size_t DM = DO > D ? D : DO;

        std::transform(other._coords.begin(), other._coords.begin() + DM, _coords.begin(), [](const TO& v) {
            return static_cast<T>(v);
        });

        std::fill(_coords.begin() + DM, _coords.end(), static_cast<T>(0));
    }

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator=(const vector<TO, DO>& other)
    {
        /*
            filling vector D with vector DO
            vector DO elements are copied to
            elements in vector D
        */

        constexpr size_t DM = DO > D ? D : DO;

        std::transform(other._coords.begin(), other._coords.begin() + DM, _coords.begin(), [](const TO& v) {
            return static_cast<T>(v);
        });

        return *this;
    }

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator=(vector<TO, DO>&& other)
    {
        /*
            filling vector D with vector DO
            vector DO elements are copied to
            elements in vector D
        */

        constexpr size_t DM = DO > D ? D : DO;

        std::transform(other._coords.begin(), other._coords.begin() + DM, _coords.begin(), [](const TO& v) {
            return static_cast<T>(v);
        });

        return *this;
    }

    //regular constructors
    
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector(const TO& v)
    {
        //initializes all vector elements with value v

        std::fill(_coords.begin(), _coords.end(), static_cast<T>(v));
    }

    template<class... TOS, typename = typename std::enable_if_t<(sizeof...(TOS) > 1) && (std::is_convertible_v<TOS, T> && ...)>>
    vector(const TOS&... vs)
    {
        constexpr size_t DM = sizeof...(TOS) > D ? D : sizeof...(TOS);

        std::array<T, sizeof...(TOS)> coords = { static_cast<T>(vs)... };

        std::copy(coords.begin(), coords.begin() + DM, _coords.begin());

        std::fill(_coords.begin() + DM, _coords.end(), static_cast<T>(0));
    }

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector(TO&& v)
    {
        //initializes all vector elements with value v

        std::fill(_coords.begin(), _coords.end(), static_cast<T>(v));
    }
    
    template<class... TOS, typename = typename std::enable_if_t<(sizeof...(TOS) > 1) && (std::is_convertible_v<TOS, T> && ...)>>
    vector(TOS&&... vs) 
    {
        constexpr size_t DM = sizeof...(TOS) > D ? D : sizeof...(TOS);

        std::array<T, sizeof...(TOS)> coords = { static_cast<T>(vs)... };

        std::copy(coords.begin(), coords.begin() + DM, _coords.begin());

        std::fill(_coords.begin() + DM, _coords.end(), static_cast<T>(0));
    }
    
    vector(std::initializer_list<T> init_list)
    {
        //fills vector with values from initializer_list element by element

        size_t DM = init_list.size() > D ? D : init_list.size();

        std::copy(init_list.begin(), init_list.begin() + DM, _coords.begin());

        //every other element is set to 0

        std::fill(_coords.begin() + DM, _coords.end(), static_cast<T>(0));
    }

    //regular operators

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator=(const TO& v)
    {
        //initializes all vector elements with value v

        std::fill(_coords.begin(), _coords.end(), static_cast<T>(v));

        return *this;
    }

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator=(TO&& v)
    {
        //initializes all vector elements with value v

        std::fill(_coords.begin(), _coords.end(), static_cast<T>(v));

        return *this;
    }
    
    vector<T, D>& operator=(std::initializer_list<T> init_list)
    {
        //fills vector with values from initializer_list element by element

        size_t DM = init_list.size() > D ? D : init_list.size();

        std::copy(init_list.begin(), init_list.begin() + DM, _coords.begin());

        return *this;
    }
public:
    //vector-vector operators

    template<class TO, size_t DO>
    auto operator+(const vector<TO, DO>& other) const
    {
        constexpr size_t DM = DO > D ? D : DO;
        using cm_t = decltype(std::declval<T>() + std::declval<TO>());

        vector<cm_t, DM> result(*this);

        for (size_t d = 0; d < DM; d++)
        {
            result._coords[d] += other._coords[d];
        }

        return result;
    }
    
    template<class TO, size_t DO>
    auto operator-(const vector<TO, DO>& other) const
    {
        constexpr size_t DM = DO > D ? D : DO;
        using cm_t = decltype(std::declval<T>() + std::declval<TO>());

        vector<cm_t, DM> result(*this);

        for (size_t d = 0; d < DM; d++)
        {
            result._coords[d] -= other._coords[d];
        }

        return result;
    }
    
    template<class TO, size_t DO>
    auto operator*(const vector<TO, DO>& other) const //shortcut for inner product
    {
        return inner_product(other);
    }

    template<class TO, size_t DO>
    vector<T, D>& operator+=(const vector<TO, DO>& other)
    {
        constexpr size_t DM = DO > D ? D : DO;

        for (size_t d = 0; d < DM; d++)
        {
            _coords[d] += static_cast<T>(other._coords[d]);
        }

        return *this;
    }

    template<class TO, size_t DO>
    vector<T, D>& operator-=(const vector<TO, DO>& other)
    {
        constexpr size_t DM = DO > D ? D : DO;
        for (size_t d = 0; d < DM; d++)
        {
            _coords[d] -= static_cast<T>(other._coords[d]);
        }
        return *this;
    }

    //vector-scalar operators

    vector<T, D> operator-() const
    {
        return *this * static_cast<T>(-1);
    }

    template<class TO, typename = typename std::enable_if_t<std::is_arithmetic_v<TO>>>
    auto operator*(const TO& v) const
    {
        using cm_t = decltype(std::declval<T>() * std::declval<TO>());

        vector<cm_t, D> result(*this);

        std::transform(result._coords.begin(), result._coords.end(), result._coords.begin(), [&v](const T& c) {
            return c * v;
        });

        return result;
    }
    
    template<class TO, typename = typename std::enable_if_t<std::is_arithmetic_v<TO>>>
    auto operator/(const TO& v) const
    {
        using cm_t = decltype(std::declval<T>() / std::declval<TO>());

        vector<cm_t, D> result(*this);

        std::transform(result._coords.begin(), result._coords.end(), result._coords.begin(), [&v](const T& c) {
            return c / v;
        });

        return result;
    }

    template<class TO, typename = typename std::enable_if_t<std::is_arithmetic_v<TO>>>
    vector<T, D>& operator*=(const TO& v)
    {
        std::transform(_coords.begin(), _coords.end(), _coords.begin(), [&v](const T& c) {
            return static_cast<T>(c * v);
        });

        return *this;
    }

    template<class TO, typename = typename std::enable_if_t<std::is_arithmetic_v<TO>>>
    vector<T, D>& operator/=(const TO& v)
    {
        std::transform(_coords.begin(), _coords.end(), _coords.begin(), [&v](const T& c) {
            return static_cast<T>(c / v);
        });

        return *this;
    }
public:
    //vector info and accessors

    constexpr size_t dimension() const
    {
        return D;
    }
    constexpr size_t size() const
    {
        return D;
    }

    T& operator[](size_t d)
    {
        return _coords[d];
    }
    const T& operator[](size_t d) const
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
public:
    //comparison operators

    //if two vectors with identical dimensions are compared, they are equal only if every corresponding element is equal
    //and inequal if there is one element that is not equal to corresponding one
    
    template<class TO>
    bool operator==(const vector<TO, D>& other) const
    {
        for (size_t d = 0; d < D; d++)
        {
            if(!equal(other._coords[d], _coords[d]))
            {
                return false;
            }
        }
        return true;
    }
    
    template<class TO>
    bool operator!=(const vector<TO, D>& other) const
    {
        for (size_t d = 0; d < D; d++)
        {
            if (!equal(other._coords[d],_coords[d]))
            {
                return true;
            }
        }
        return false;
    }

    //else vectors are always inequal

    template<class TO, size_t DO>
    bool operator==(const vector<TO, DO>& other) const
    {
        return false;
    }

    template<class TO, size_t DO>
    bool operator!=(const vector<TO, DO>& other) const
    {
        return true;
    }
public:
    template<typename = typename std::enable_if_t<D >= 1 >>
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

    template<class TO, typename = typename std::enable_if_t<D == 3>>
    vector<T, D> cross_productLH(const vector<TO, D>& other) const
    {
        return {
            _coords[2] * static_cast<T>(other._coords[1]) - _coords[1] * static_cast<T>(other._coords[2]),
            _coords[2] * static_cast<T>(other._coords[0]) - _coords[0] * static_cast<T>(other._coords[2]),
            _coords[1] * static_cast<T>(other._coords[0]) -_coords[0] * static_cast<T>(other._coords[1])
        };
    }
    template<class TO, typename = typename std::enable_if_t<D == 3>>
    vector<T, D> cross_productRH(const vector<TO, D>& other) const
    {
        return {
            _coords[1] * static_cast<T>(other._coords[2]) - _coords[2] * static_cast<T>(other._coords[1]),
            _coords[0] * static_cast<T>(other._coords[2]) - _coords[2] * static_cast<T>(other._coords[0]),
            _coords[0] * static_cast<T>(other._coords[1]) - _coords[1] * static_cast<T>(other._coords[0])
        };
    }

    inline auto magnitude() const
    {
        return std::sqrt(magnitude_sqr());
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
    
    template<class TO>
    inline auto inner_product(const vector<TO,D>& other) const
    {
        using cm_t = decltype(std::declval<TO>() * std::declval<T>() + std::declval<TO>() * std::declval<T>());
        return std::inner_product(_coords.begin(), _coords.end(), other._coords.begin(), static_cast<cm_t>(0));
    }
public:
    template<class TO, size_t DO>
    friend std::ostream& operator<<(std::ostream& os, const vector<TO, DO>& vector);
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
};

template<class TO, size_t DO>
std::ostream& operator<<(std::ostream& os, const vector<TO, DO>& vector)
{
    os << "(";
    for (size_t d = 0; d < DO - 1; d++)
    {
        os << vector._coords[d] << ", ";
    }
    os << vector._coords[DO - 1] << ")";
    return os;
}

template<class T, size_t D, class TO, typename = typename std::enable_if_t<std::is_arithmetic_v<TO>>>
inline auto operator*(const TO& v, const vector<T, D>& vector)
{
    return vector * v;
}

template<class T, size_t D, class TO, typename = typename std::enable_if_t<std::is_arithmetic_v<TO>>>
inline auto operator/(const TO& v, const vector<T, D>& vector)
{
    return vector / v;
}

NAMESPACE_LINEAR_ALGEBRA_END