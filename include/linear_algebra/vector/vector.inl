#pragma once

#include "vector.hpp"

NAMESPACE_LINEAR_ALGEBRA_BEGIN

//static storage

template<class T, size_t D>
inline T& vector<T, D>::vector_storage_static::operator[](size_t d)
{
    return _coords[d];
}

template<class T, size_t D>
inline const T& vector<T, D>::vector_storage_static::operator[](size_t d) const
{
    return _coords[d];
}

template<class T, size_t D>
inline T* vector<T, D>::vector_storage_static::data()
{
    return NULL;
}

template<class T, size_t D>
inline const T* vector<T, D>::vector_storage_static::data() const
{
    return _coords;
}

template<class T, size_t D>
inline T* vector<T, D>::vector_storage_static::begin()
{
    return _coords;
}

template<class T, size_t D>
inline const T* vector<T, D>::vector_storage_static::begin() const
{
    return _coords;
}

template<class T, size_t D>
inline T* vector<T, D>::vector_storage_static::end()
{
    return _coords + D;
}

template<class T, size_t D>
inline const T* vector<T, D>::vector_storage_static::end() const
{
    return _coords + D;
}

//dynamic storage

template<class T, size_t D>
inline vector<T, D>::vector_storage_dynamic::vector_storage_dynamic()
{
    _coords = new T[D];
}

template<class T, size_t D>
inline vector<T, D>::vector_storage_dynamic::vector_storage_dynamic(typename vector<T, D>::vector_storage_dynamic&& other)
{
    _coords = other._coords;
    other._coords = nullptr;
}

template<class T, size_t D>
inline typename vector<T, D>::vector_storage_dynamic& vector<T, D>::vector_storage_dynamic::operator=(typename vector<T, D>::vector_storage_dynamic&& other)
{
    _coords = other._coords;
    other._coords = nullptr;
    return *this;
}

template<class T, size_t D>
inline vector<T, D>::vector_storage_dynamic::~vector_storage_dynamic()
{
    if (_coords)
    {
        delete[] _coords;
    }
}

template<class T, size_t D>
inline T& vector<T, D>::vector_storage_dynamic::operator[](size_t d)
{
    return _coords[d];
}

template<class T, size_t D>
inline const T& vector<T, D>::vector_storage_dynamic::operator[](size_t d) const
{
    return _coords[d];
}

template<class T, size_t D>
inline T* vector<T, D>::vector_storage_dynamic::data()
{
    return _coords;
}

template<class T, size_t D>
inline const T* vector<T, D>::vector_storage_dynamic::data() const
{
    return _coords;
}

template<class T, size_t D>
inline T* vector<T, D>::vector_storage_dynamic::begin()
{
    return _coords;
}

template<class T, size_t D>
inline const T* vector<T, D>::vector_storage_dynamic::begin() const
{
    return _coords;
}

template<class T, size_t D>
inline T* vector<T, D>::vector_storage_dynamic::end()
{
    return _coords + D;
}

template<class T, size_t D>
inline const T* vector<T, D>::vector_storage_dynamic::end() const
{
    return _coords + D;
}

//vector

template<class T, size_t D>
vector<T,D>::vector()
{
    //filling vector to initialize it with 0-oes
    std::fill(_coords.begin(), _coords.end(), get_additive_identity<T>());
}

template<class T, size_t D>
vector<T, D>::vector(const vector<T,D>& other)
{
    std::copy(other._coords.begin(), other._coords.end(), _coords.begin());
}

template<class T, size_t D>
vector<T, D>::vector(vector<T, D>&& other)
{
    /*
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

template<class T, size_t D>
vector<T, D>& vector<T, D>::operator=(const vector<T, D>& other)
{
    std::copy(other._coords.begin(), other._coords.end(), _coords.begin());

    return *this;
}

template<class T, size_t D>
vector<T, D>& vector<T, D>::operator=(vector<T, D>&& other)
{
    /*
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

template<class T, size_t D>
template<class TO, typename>
vector<T, D>::vector(const vector<TO, D>& other)
{
    std::transform(other._coords.begin(), other._coords.end(), _coords.begin(), [](const TO& v) {
        return static_cast<T>(v);
    });
}

template<class T, size_t D>
template<class TO, typename>
vector<T, D>::vector(vector<TO, D>&& other)
{
    std::transform(other._coords.begin(), other._coords.end(), _coords.begin(), [](const TO& v) {
        return static_cast<T>(v);
    });
}

template<class T, size_t D>
template<class TO, typename>
vector<T, D>& vector<T, D>::operator=(const vector<TO, D>& other)
{
    std::transform(other._coords.begin(), other._coords.end(), _coords.begin(), [](const TO& v) {
        return static_cast<T>(v);
    });

    return *this;
}

template<class T, size_t D>
template<class TO, typename>
vector<T, D>& vector<T, D>::operator=(vector<TO, D>&& other)
{
    std::transform(other._coords.begin(), other._coords.end(), _coords.begin(), [](const TO& v) {
        return static_cast<T>(v);
    });

    return *this;
}

template<class T, size_t D>
template<class TO, size_t DO, typename>
vector<T, D>::vector(const vector<TO, DO>& other)
{
    std::transform(other._coords.begin(), other._coords.begin() + smaller<D, DO>, _coords.begin(), [](const TO& v) {
        return static_cast<T>(v);
    });

    std::fill(_coords.begin() + smaller<D, DO>, _coords.end(), get_additive_identity<T>());
}

template<class T, size_t D>
template<class TO, size_t DO, typename>
vector<T, D>::vector(vector<TO, DO>&& other)
{
    std::transform(other._coords.begin(), other._coords.begin() + smaller<D, DO>, _coords.begin(), [](const TO& v) {
        return static_cast<T>(v);
    });

    std::fill(_coords.begin() + smaller<D, DO>, _coords.end(), get_additive_identity<T>());
}

template<class T, size_t D>
template<class TO, size_t DO, typename>
vector<T, D>& vector<T, D>::operator=(const vector<TO, DO>& other)
{
    std::transform(other._coords.begin(), other._coords.begin() + smaller<D, DO>, _coords.begin(), [](const TO& v) {
        return static_cast<T>(v);
    });

    return *this;
}

template<class T, size_t D>
template<class TO, size_t DO, typename>
vector<T, D>& vector<T, D>::operator=(vector<TO, DO>&& other)
{
    std::transform(other._coords.begin(), other._coords.begin() + smaller<D, DO>, _coords.begin(), [](const TO& v) {
        return static_cast<T>(v);
    });

    return *this;
}

template<class T, size_t D>
template<class TO, typename>
vector<T, D>::vector(const TO& v)
{
    //initializes all vector elements with value v

    std::fill(_coords.begin(), _coords.end(), static_cast<T>(v));
}

template<class T, size_t D>
template<class... TOS, typename>
vector<T, D>::vector(const TOS&... vs)
{
    constexpr size_t DM = smaller<D, sizeof...(TOS)>;

    std::array<T, sizeof...(TOS)> coords = { static_cast<T>(vs)... };

    std::copy(coords.begin(), coords.begin() + DM, _coords.begin());

    std::fill(_coords.begin() + DM, _coords.end(), get_additive_identity<T>());
}

template<class T, size_t D>
template<class TO, typename>
vector<T, D>::vector(TO&& v)
{
    //initializes all vector elements with value v

    std::fill(_coords.begin(), _coords.end(), static_cast<T>(v));
}

template<class T, size_t D>
template<class... TOS, typename>
vector<T, D>::vector(TOS&&... vs)
{
    constexpr size_t DM = smaller<D, sizeof...(TOS)>;

    std::array<T, sizeof...(TOS)> coords = { static_cast<T>(vs)... };

    std::copy(coords.begin(), coords.begin() + DM, _coords.begin());

    std::fill(_coords.begin() + DM, _coords.end(), get_additive_identity<T>());
}

template<class T, size_t D>
vector<T, D>::vector(std::initializer_list<T> init_list)
{
    //fills vector with values from initializer_list element by element

    size_t DM = init_list.size() > D ? D : init_list.size();

    std::copy(init_list.begin(), init_list.begin() + DM, _coords.begin());

    //every other element is set to 0

    std::fill(_coords.begin() + DM, _coords.end(), get_additive_identity<T>());
}

template<class T, size_t D>
template<class TO, typename>
vector<T, D>::vector(std::initializer_list<TO> init_list)
{
    //fills vector with values from initializer_list element by element

    size_t DM = init_list.size() > D ? D : init_list.size();

    std::transform(init_list.begin(), init_list.begin() + DM, _coords.begin(), [](const TO& v) {
        return static_cast<T>(v);
    });

    //every other element is set to 0

    std::fill(_coords.begin() + DM, _coords.end(), get_additive_identity<T>());
}

template<class T, size_t D>
template<class TO, typename>
vector<T, D>& vector<T, D>::operator=(const TO& v)
{
    //initializes all vector elements with value v

    std::fill(_coords.begin(), _coords.end(), static_cast<T>(v));

    return *this;
}

template<class T, size_t D>
template<class TO, typename>
vector<T, D>& vector<T, D>::operator=(TO&& v)
{
    //initializes all vector elements with value v

    std::fill(_coords.begin(), _coords.end(), static_cast<T>(v));

    return *this;
}

template<class T, size_t D>
vector<T, D>& vector<T, D>::operator=(std::initializer_list<T> init_list)
{
    //fills vector with values from initializer_list element by element

    size_t DM = init_list.size() > D ? D : init_list.size();

    std::copy(init_list.begin(), init_list.begin() + DM, _coords.begin());

    return *this;
}

template<class T, size_t D>
template<class TO, typename>
vector<T, D>& vector<T, D>::operator=(std::initializer_list<TO> init_list)
{
    //fills vector with values from initializer_list element by element

    size_t DM = init_list.size() > D ? D : init_list.size();

    std::transform(init_list.begin(), init_list.begin() + DM, _coords.begin(), [](const TO& v) {
        return static_cast<T>(v);
    });

    return *this;
}

template<class T, size_t D>
template<class TO, size_t DO, typename>
vector<addition_result_t<T, TO>, smaller<D, DO>> vector<T, D>::operator+(const vector<TO, DO>& other) const
{
    vector<addition_result_t<T, TO>, smaller<D, DO>> result;

    for (size_t d = 0; d < smaller<D, DO>; d++)
    {
        result._coords[d] = _coords[d] + other._coords[d];
    }

    return result;
}

template<class T, size_t D>
template<class TO, size_t DO, typename>
vector<subtraction_result_t<T, TO>, smaller<D, DO>> vector<T, D>::operator-(const vector<TO, DO>& other) const
{
    vector<addition_result_t<T, TO>, smaller<D, DO>> result;
    for (size_t d = 0; d < smaller<D, DO>; d++)
    {
        result._coords[d] = _coords[d] - other._coords[d];
    }

    return result;
}

template<class T, size_t D>
template<class TO, size_t DO, typename>
inner_product_result_t<T, TO> vector<T, D>::operator*(const vector<TO, DO>& other) const
{
    return inner_product(other);
}

template<class T, size_t D>
template<class TO, size_t DO, typename>
vector<T, D>& vector<T, D>::operator+=(const vector<TO, DO>& other)
{
    for (size_t d = 0; d < smaller<D, DO>; d++)
    {
        _coords[d] += static_cast<T>(other._coords[d]);
    }

    return *this;
}

template<class T, size_t D>
template<class TO, size_t DO, typename>
vector<T, D>& vector<T, D>::operator-=(const vector<TO, DO>& other)
{
    for (size_t d = 0; d < smaller<D, DO>; d++)
    {
        _coords[d] -= static_cast<T>(other._coords[d]);
    }

    return *this;
}

template<class T, size_t D>
vector<T, D> vector<T, D>::operator-() const
{
    vector<T, D> result;

    for (size_t d = 0; d < D; d++)
    {
        result._coords[d] = -_coords[d];
    }

    return result;
}

template<class T, size_t D>
template<class TO, typename>
vector<multiplication_result_t<T, TO>, D> vector<T, D>::operator*(const TO& v) const
{
    vector<multiplication_result_t<T, TO>, D> result;

    for (size_t d = 0; d < D; d++)
    {
        result._coords[d] = _coords[d] * v;
    }

    return result;
}

template<class T, size_t D>
template<class TO, typename>
vector<division_result_t<T, TO>, D> vector<T, D>::operator/(const TO& v) const
{
    vector<division_result_t<T, TO>, D> result;

    for (size_t d = 0; d < D; d++)
    {
        result._coords[d] = _coords[d] / v;
    }

    return result;
}

template<class T, size_t D, class TO, typename = typename std::enable_if_t<can_be_multiplied_v<T, TO> && std::is_convertible_v<TO, T>>>
vector<multiplication_result_t<T, TO>, D> operator*(const TO& v, const vector<T, D>& vector)
{
    return vector * v;
}

template<class T, size_t D>
template<class TO, typename>
vector<T, D>& vector<T, D>::operator*=(const TO& v)
{
    for (size_t d = 0; d < D; d++)
    {
        _coords[d] = _coords[d] * static_cast<T>(v);
    }

    return *this;
}

template<class T, size_t D>
template<class TO, typename>
vector<T, D>& vector<T, D>::operator/=(const TO& v)
{
    for (size_t d = 0; d < D; d++)
    {
        _coords[d] = _coords[d] / static_cast<T>(v);
    }

    return *this;
}

template<class T, size_t D>
T& vector<T, D>::operator[](size_t d)
{
    return _coords[d];
}

template<class T, size_t D>
const T& vector<T, D>::operator[](size_t d) const
{
    return _coords[d];
}

template<class T, size_t D>
T* vector<T, D>::data()
{
    return _coords;
}

template<class T, size_t D>
const T* vector<T, D>::data() const
{
    return _coords;
}

template<class T, size_t D>
T* vector<T, D>::begin()
{
    return _coords.begin();
}

template<class T, size_t D>
const T* vector<T, D>::begin() const
{
    return _coords.begin();
}

template<class T, size_t D>
T* vector<T, D>::end()
{
    return _coords.end();
}

template<class T, size_t D>
const T* vector<T, D>::end() const
{
    return _coords.end();
}

template<class T, size_t D>
template<class TO, size_t DO, typename>
bool vector<T, D>::operator==(const vector<TO, DO>& other) const
{
    if constexpr (D == DO)
    {
        for (size_t d = 0; d < D; d++)
        {
            if (!equal(other._coords[d], _coords[d]))
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

template<class T, size_t D>
template<class TO, size_t DO, typename>
bool vector<T, D>::operator!=(const vector<TO, DO>& other) const
{
    if constexpr (D == DO)
    {
        for (size_t d = 0; d < D; d++)
        {
            if (inequal(other._coords[d], _coords[d]))
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        return true;
    }
}

template<class T, size_t D>
template<typename>
T& vector<T, D>::x()
{
    return _coords[0];
}

template<class T, size_t D>
template<typename>
const T & vector<T, D>::x() const
{
    return _coords[0];
}

template<class T, size_t D>
template<typename>
void vector<T, D>::set_x(const T& x)
{
    _coords[0] = x;
}

template<class T, size_t D>
template<typename>
T& vector<T, D>::y()
{
    return _coords[1];
}

template<class T, size_t D>
template<typename>
const T & vector<T, D>::y() const
{
    return _coords[1];
}

template<class T, size_t D>
template<typename>
void vector<T, D>::set_y(const T& y)
{
    _coords[1] = y;
}

template<class T, size_t D>
template<typename>
T& vector<T, D>::z()
{
    return _coords[2];
}

template<class T, size_t D>
template<typename>
const T& vector<T, D>::z() const
{
    return _coords[2];
}

template<class T, size_t D>
template<typename>
void vector<T, D>::set_z(const T& z)
{
    _coords[2] = z;
}

template<class T, size_t D>
template<typename>
T& vector<T, D>::w()
{
    return _coords[3];
}

template<class T, size_t D>
template<typename>
const T& vector<T, D>::w() const
{
    return _coords[3];
}

template<class T, size_t D>
template<typename>
void vector<T, D>::set_w(const T& w)
{
    _coords[3] = w;
}

template<class T, size_t D>
template<size_t DO, typename>
vector<T, D> vector<T, D>::perpendicularLH() const
{
    return vector<T, D>(_coords[1], -_coords[0]);
}

template<class T, size_t D>
template<size_t DO, typename>
vector<T, D> vector<T, D>::perpendicularRH() const
{
    return vector<T, D>(-_coords[1], _coords[0]);
}

template<class T, size_t D>
template<class TO, typename>
vector<T, D> vector<T, D>::cross_productLH(const vector<TO, D>& other) const
{
    return vector<T, D>(
        _coords[2] * static_cast<T>(other._coords[1]) - _coords[1] * static_cast<T>(other._coords[2]),
        _coords[2] * static_cast<T>(other._coords[0]) - _coords[0] * static_cast<T>(other._coords[2]),
        _coords[1] * static_cast<T>(other._coords[0]) - _coords[0] * static_cast<T>(other._coords[1])
    );
}

template<class T, size_t D>
template<class TO, typename>
vector<T, D> vector<T, D>::cross_productRH(const vector<TO, D>& other) const
{
    return vector<T, D>(
        _coords[1] * static_cast<T>(other._coords[2]) - _coords[2] * static_cast<T>(other._coords[1]),
        _coords[0] * static_cast<T>(other._coords[2]) - _coords[2] * static_cast<T>(other._coords[0]),
        _coords[0] * static_cast<T>(other._coords[1]) - _coords[1] * static_cast<T>(other._coords[0])
    );
}

template<class T, size_t D>
template<typename>
T vector<T, D>::magnitude() const
{
    return functions_implementation<T>::sqrt(magnitude_sqr());
}

template<class T, size_t D>
template<typename>
T vector<T, D>::length() const
{
    return magnitude();
}

template<class T, size_t D>
T vector<T, D>::magnitude_sqr() const
{
    return std::inner_product(_coords.begin(), _coords.end(), _coords.begin(), get_additive_identity<T>());
}

template<class T, size_t D>
T vector<T, D>::length_sqr() const
{
    return magnitude_sqr();
}

template<class T, size_t D>
template<typename>
T vector<T, D>::normalized() const
{
    return *this / length();
}

template<class T, size_t D>
template<typename>
vector<T, D>& vector<T, D>::normalize()
{
    *this /= length();
    return *this;
}

template<class T, size_t D>
template<class TO, typename>
inner_product_result_t<T, TO> vector<T, D>::inner_product(const vector<TO, D>& other) const
{
    return std::inner_product(_coords.begin(), _coords.end(), other._coords.begin(), get_additive_identity<inner_product_result_t<T, TO>>());
}

NAMESPACE_LINEAR_ALGEBRA_IO_BEGIN

template<class TO, size_t DO>
std::ostream& operator<<(std::ostream& os, const vector<TO, DO>& v)
{
    os << "(";
    for (size_t d = 0; d < DO - 1; d++)
    {
        os << v._coords[d] << ", ";
    }
    os << v._coords[DO - 1] << ")";
    return os;
}

NAMESPACE_LINEAR_ALGEBRA_IO_END

template<class T, size_t D>
const vector<T, D>& vector<T, D>::zero()
{
    static const vector<T, D> z = vector<T, D>(get_additive_identity<T>());
    return z;
}

template<class T, size_t D>
const vector<T, D>& vector<T, D>::one()
{
    static const vector<T, D> o = vector<T, D>(get_multiplicative_identity<T>);
    return o;
}

NAMESPACE_LINEAR_ALGEBRA_END

