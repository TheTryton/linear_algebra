#pragma once

#include "triangle.hpp"

NAMESPACE_GEOMETRY2D_BEGIN

template<class T>
triangle<T>::triangle() :
    _points({ point_type<T>(), point_type<T>(), point_type<T>() })
{
}

template<class T>
triangle<T>::triangle(const T & x1, const T & y1, const T & x2, const T & y2, const T & x3, const T & y3):
    _points({ point_type<T>(x1, y1), point_type<T>(x2, y2), point_type<T>(x3, y3) })
{
}

template<class T>
triangle<T>::triangle(const point_type<T>& v1, const point_type<T>& v2, const point_type<T>& v3) :
    _points({ v1,v2,v3 })
{
}

template<class T>
triangle<T>::triangle(std::initializer_list<point_type<T>> init_list)
{
    size_t C = init_list.size() > 3 ? 3 : init_list.size();
    std::copy(init_list.begin(), init_list.begin() + C, _points);
}

template<class T>
triangle<T>::triangle(std::initializer_list<T> init_list)
{
    if (init_list.size() == 1)
    {
        _points[0][0] = *init_list.begin();
        if (init_list.size() == 2)
        {
            _points[0][1] = *(init_list.begin() + 1);
            if (init_list.size() == 3)
            {
                _points[1][0] = *(init_list.begin() + 2);
                if (init_list.size() == 4)
                {
                    _points[1][1] = *(init_list.begin() + 3);
                    if (init_list.size() == 5)
                    {
                        _points[2][0] = *(init_list.begin() + 4);
                        if (init_list.size() == 6)
                        {
                            _points[2][1] = *(init_list.begin() + 5);
                        }
                    }
                }
            }
        }
    }
}

template<class T>
point_type<T>* triangle<T>::data()
{
    return _points.data();
}

template<class T>
const point_type<T>* triangle<T>::data() const
{
    return _points.data();
}

template<class T>
constexpr size_t triangle<T>::size() const
{
    return _points.size();
}

template<class T>
point_type<T>& triangle<T>::operator[](size_t i)
{
    return _points[i];
}

template<class T>
const point_type<T>& triangle<T>::operator[](size_t i) const
{
    return _points[i];
}

template<class T>
auto triangle<T>::begin()
{
    return _points.begin();
}

template<class T>
auto triangle<T>::begin() const
{
    return _points.begin();
}

template<class T>
auto triangle<T>::end()
{
    return _points.end();
}

template<class T>
auto triangle<T>::end() const
{
    return _points.end();
}

template<class T>
point_type<T> triangle<T>::center() const
{
    return (_points[0] + _points[1] + _points[2]) / static_cast<T>(3);
}

template<class T>
point_type<T>& triangle<T>::p1()
{
    return _points[0];
}

template<class T>
const point_type<T>& triangle<T>::p1() const
{
    return _points[0];
}

template<class T>
void triangle<T>::set_p1(const point_type<T>& p)
{
    _points[0] = p;
}

template<class T>
point_type<T>& triangle<T>::p2()
{
    return _points[1];
}

template<class T>
const point_type<T>& triangle<T>::p2() const
{
    return _points[1];
}

template<class T>
void triangle<T>::set_p2(const point_type<T>& p)
{
    _points[1] = p;
}

template<class T>
point_type<T>& triangle<T>::p3()
{
    return _points[2];
}

template<class T>
const point_type<T>& triangle<T>::p3() const
{
    return _points[2];
}

template<class T>
void triangle<T>::set_p3(const point_type<T>& p)
{
    _points[2] = p;
}

template<class T>
inline T perp_dot_product(const point_type<T>& v1, const point_type<T>& v2)
{
    return v1[0] * v2[1] - v1[1] * v2[0];
}

template<class T>
template<inclusion_mode mode>
bool triangle<T>::contains(const point_type<T>& p) const
{
    //plane way
    
    auto p1p2 = _points[1] - _points[0];
    auto p1p3 = _points[2] - _points[0];
    auto p1p = p - _points[0];

    auto w = p1p2[0] * p1p3[1] - p1p2[1] * p1p3[0];
    if(std::abs(w) <= std::numeric_limits<T>::epsilon())
    {
        return segment<T>(_points[0], _points[1]).contains<mode>(p, static_cast<T>(1));
    }

    auto ws = p1p[0] * p1p3[1] - p1p[1] * p1p3[0];
    auto wt = p1p[1] * p1p2[0] - p1p[0] * p1p2[1];

    auto s = ws / w;
    auto t = wt / w;

    if constexpr (mode == inclusion_mode::full)
    {
        return s >= static_cast<T>(0) && t >= static_cast<T>(0) && (s + t) <= static_cast<T>(1);
    }
    if constexpr (mode == inclusion_mode::edgeless)
    {
        return s > static_cast<T>(0) && t > static_cast<T>(0) && (s + t) < static_cast<T>(1);
    }
    if constexpr (mode == inclusion_mode::edge_only)
    {
        return s == static_cast<T>(0) && t == static_cast<T>(0) && (s + t) == static_cast<T>(1);
    }

    return false;
}

template<class T>
bool triangle<T>::operator==(const triangle& other) const
{
    for (size_t d = 0; d < 3; d++)
    {
        if (other._points[d] != _points[d])
        {
            return false;
        }
    }
    return true;
}

template<class T>
bool triangle<T>::operator!=(const triangle& other) const
{
    for (size_t d = 0; d < 3; d++)
    {
        if (other._points[d] != _points[d])
        {
            return true;
        }
    }
    return false;
}

NAMESPACE_GEOMETRY2D_END