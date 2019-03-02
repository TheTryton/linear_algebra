#pragma once

#include "line.hpp"
#include "segment.hpp"

NAMESPACE_GEOMETRY2D_BEGIN

template<class T>
template<class D>
line<T>::line(const segment<D>& other)
{
    std::transform(other._points.begin(), other._points.end(), _points.begin(), [](const geometry2D::point_type<D>& v) {
        return static_cast<geometry2D::point_type<T>>(v);
    });
}

template<class T>
template<class D>
line<T>::line(segment<D>&& other)
{
    std::transform(other._points.begin(), other._points.end(), _points.begin(), [](const geometry2D::point_type<D>& v) {
        return static_cast<geometry2D::point_type<T>>(v);
    });
}

template<class T>
template<class D>
line<T>& line<T>::operator=(const segment<D>& other)
{
    std::transform(other._points.begin(), other._points.end(), _points.begin(), [](const geometry2D::point_type<D>& v) {
        return static_cast<geometry2D::point_type<T>>(v);
    });
    return *this;
}

template<class T>
template<class D>
line<T>& line<T>::operator=(segment<D>&& other)
{
    std::transform(other._points.begin(), other._points.end(), _points.begin(), [](const geometry2D::point_type<D>& v) {
        return static_cast<geometry2D::point_type<T>>(v);
    });
    return *this;
}

template<class T>
template<class D>
segment<T>::segment(const line<D>& other)
{
    std::transform(other._points.begin(), other._points.end(), _points.begin(), [](const geometry2D::point_type<D>& v) {
        return static_cast<geometry2D::point_type<T>>(v);
    });
}

template<class T>
template<class D>
segment<T>::segment(line<D>&& other)
{
    std::transform(other._points.begin(), other._points.end(), _points.begin(), [](const geometry2D::point_type<D>& v) {
        return static_cast<geometry2D::point_type<T>>(v);
    });
}

template<class T>
template<class D>
segment<T>& segment<T>::operator=(const line<D>& other)
{
    std::transform(other._points.begin(), other._points.end(), _points.begin(), [](const geometry2D::point_type<D>& v) {
        return static_cast<geometry2D::point_type<T>>(v);
    });
    return *this;
}

template<class T>
template<class D>
segment<T>& segment<T>::operator=(line<D>&& other)
{
    std::transform(other._points.begin(), other._points.end(), _points.begin(), [](const geometry2D::point_type<D>& v) {
        return static_cast<geometry2D::point_type<T>>(v);
    });
    return *this;
}

template<class T>
template<class D>
line<T>::operator segment<D>() const
{
    return segment<D>(*this);
}

template<class T>
template<class D>
segment<T>::operator line<D>() const
{
    return line<D>(*this);
}

NAMESPACE_GEOMETRY2D_END