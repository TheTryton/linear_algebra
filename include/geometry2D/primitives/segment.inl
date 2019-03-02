#pragma once

#include "segment.hpp"
#include "line_segment_common.inl"
#include "../projections/projections.hpp"
#include "../projections/projections.inl"

NAMESPACE_GEOMETRY2D_BEGIN

template<class T>
segment<T>::segment() :
    _points({ point_type<T>(),point_type<T>() })
{
}

template<class T>
segment<T>::segment(const T& x1, const T& y1, const T& x2, const T& y2) :
    _points({ point_type<T>(x1, y1),point_type<T>(x2, y2) })
{
}

template<class T>
segment<T>::segment(const point_type<T>& v1, const point_type<T>& v2) :
    _points({ v1,v2 })
{
}

template<class T>
segment<T>::segment(std::initializer_list<point_type<T>> init_list)
{
    size_t C = init_list.size() > _points.size() ? _points.size() : init_list.size();
    std::copy(init_list.begin(), init_list.begin() + C, _points.begin());
}

template<class T>
segment<T>::segment(std::initializer_list<T> init_list)
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
                }
            }
        }
    }
}

template<class T>
template<class D>
inline segment<T>::segment(const segment<D>& other)
{
    std::transform(other._points.begin(), other._points.end(), _points.begin(), [](const D& v) {
        return static_cast<T>(v);
    });
}

template<class T>
template<class D>
segment<T>::segment(segment<D>&& other)
{
    std::transform(other._points.begin(), other._points.end(), _points.begin(), [](const D& v) {
        return static_cast<T>(v);
    });
}

template<class T>
template<class D>
segment<T>& segment<T>::operator=(const segment<D>& other)
{
    std::transform(other._points.begin(), other._points.end(), _points.begin(), [](const D& v) {
        return static_cast<T>(v);
    });
    return *this;
}

template<class T>
template<class D>
segment<T>& segment<T>::operator=(segment<D>&& other)
{
    std::transform(other._points.begin(), other._points.end(), _points.begin(), [](const D& v) {
        return static_cast<T>(v);
    });
    return *this;
}

template<class T>
point_type<T>* segment<T>::data()
{
    return _points.data();
}

template<class T>
const point_type<T>* segment<T>::data() const
{
    return _points.data();
}

template<class T>
constexpr size_t segment<T>::size() const
{
    return _points.size();
}

template<class T>
point_type<T>& segment<T>::operator[](size_t i)
{
    return _points[i];
}

template<class T>
const point_type<T>& segment<T>::operator[](size_t i) const
{
    return _points[i];
}

template<class T>
auto segment<T>::begin()
{
    return _points.begin();
}

template<class T>
auto segment<T>::begin() const
{
    return _points.begin();
}

template<class T>
auto segment<T>::end()
{
    return _points.end();
}

template<class T>
auto segment<T>::end() const
{
    return _points.end();
}

template<class T>
point_type<T> segment<T>::center() const
{
    return (_points[0] + _points[1]) / static_cast<T>(2);
}

template<class T>
point_type<T> segment<T>::evaluate(const T& x) const
{
    return _points[0] * (static_cast<T>(1) - x) + _points[1] * x;
}

template<class T>
point_type<T>& segment<T>::p1()
{
    return _points[0];
}

template<class T>
const point_type<T>& segment<T>::p1() const
{
    return _points[0];
}

template<class T>
void segment<T>::set_p1(const point_type<T>& p)
{
    _points[0] = p;
}

template<class T>
point_type<T>& segment<T>::p2()
{
    return _points[1];
}

template<class T>
const point_type<T>& segment<T>::p2() const
{
    return _points[1];
}

template<class T>
void segment<T>::set_p2(const point_type<T>& p)
{
    _points[1] = p;
}

template<class T>
T segment<T>::length() const
{
    return (_points[0] - _points[1]).length();
}

template<class T>
template<inclusion_mode mode>
bool segment<T>::contains(const point_type<T>& p, const T& segment_width) const
{
    if constexpr (mode == inclusion_mode::full)
    {
        return (p -projection(p, *this)).length() <= segment_width;
    }
    if constexpr (mode == inclusion_mode::edgeless)
    {
        return (p - projection(p, *this)).length() < segment_width;
    }
    if constexpr (mode == inclusion_mode::edge_only)
    {
        return (p - projection(p, *this)).length() == segment_width;
    }
    return false;
}

template<class T>
void segment<T>::translate(const T& dx, const T& dy)
{
    _points[0][0] += dx;
    _points[1][0] += dx;
    _points[0][1] += dy;
    _points[1][1] += dy;
}

template<class T>
void segment<T>::translate(const vector_type<T>& v)
{
    _points[0][0] += v[0];
    _points[1][0] += v[0];
    _points[0][1] += v[1];
    _points[1][1] += v[1];
}

template<class T>
segment<T> segment<T>::translated(const T& dx, const T& dy) const
{
    return segment(point_type<T>(_points[0][0] + dx, _points[0][1] + dy), point_type<T>(_points[1][0] + dx, _points[1][1] + dy));
}

template<class T>
segment<T> segment<T>::translated(const vector_type<T>& v) const
{
    return segment(point_type<T>(_points[0][0] + v[0], _points[0][1] + v[1]), point_type<T>(_points[1][0] + v[0], _points[1][1] + v[1]));
}

template<class T>
bool segment<T>::operator==(const segment& other) const
{
    for (uint d = 0; d < 2; d++)
    {
        if (other._points[d] != _points[d])
        {
            return false;
        }
    }
    return true;
}

template<class T>
bool segment<T>::operator!=(const segment& other) const
{
    for (uint d = 0; d < 2; d++)
    {
        if (other._points[d] != _points[d])
        {
            return true;
        }
    }
    return false;
}

NAMESPACE_GEOMETRY2D_END