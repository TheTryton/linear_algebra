#pragma once

#include "rect.hpp"

NAMESPACE_GEOMETRY2D_BEGIN

template<class T>
rect<T>::rect() :
    _points({ point_type<T>(),point_type<T>() })
{
}

template<class T>
rect<T>::rect(const point_type<T>& bottom_left, const point_type<T>& top_right) :
    _points{ bottom_left, top_right }
{
}

template<class T>
rect<T>::rect(const T & x, const T & y, const T & w, const T & h) :
    _points{ point_type<T>(x, y), point_type<T>(x + w, y + h) }
{
}

template<class T>
rect<T>::rect(std::initializer_list<point_type<T>> init_list)
{
    size_t C = init_list.size() > _points.size() ? _points.size() : init_list.size();
    std::copy(init_list.begin(), init_list.begin() + C, _points.begin());
}


template<class T>
rect<T>::rect(std::initializer_list<T> init_list)
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
rect<T>::rect(const rect<D>& other)
{
    std::copy(other._points, other._points + 2, _points);
}

template<class T>
template<class D>
rect<T>::rect(rect<D>&& other)
{
    std::copy(other._points, other._points + 2, _points);
}

template<class T>
template<class D>
rect<T>& rect<T>::operator=(const rect<D>& other)
{
    std::copy(other._points, other._points + 2, _points);
    return *this;
}

template<class T>
template<class D>
rect<T>& rect<T>::operator=(rect<D>&& other)
{
    std::copy(other._points, other._points + 2, _points);
    return *this;
}

template<class T>
point_type<T>* rect<T>::data()
{
    return _points;
}

template<class T>
const point_type<T>* rect<T>::data() const
{
    return _points;
}

template<class T>
constexpr size_t rect<T>::size() const
{
    return _points.size();
}

template<class T>
point_type<T>& rect<T>::operator[](size_t i)
{
    return _points[i];
}

template<class T>
const point_type<T>& rect<T>::operator[](size_t i) const
{
    return _points[i];
}

template<class T>
auto rect<T>::begin()
{
    return _points.begin();
}

template<class T>
auto rect<T>::begin() const
{
    return _points.begin();
}

template<class T>
auto rect<T>::end()
{
    return _points.end();
}

template<class T>
auto rect<T>::end() const
{
    return _points.end();
}

template<class T>
void rect<T>::adjust(const T & dlx, const T & duy, const T & drx, const T & ddy)
{
    _points[0][0] = _points[0][0] + dlx;
    _points[0][1] = _points[0][1] + duy;
    _points[1][0] = _points[1][0] + drx;
    _points[1][1] = _points[1][1] + ddy;
}

template<class T>
rect<T> rect<T>::adjusted(const T & dlx, const T & duy, const T & drx, const T & ddy) const
{
    return rect(_points[0][0] + dlx, _points[0][1] + duy, _points[1][0] + drx, _points[1][1] + ddy);
}

template<class T>
T& rect<T>::left()
{
    return _points[0][0];
}

template<class T>
const T& rect<T>::left() const
{
    return _points[0][0];
}

template<class T>
void rect<T>::set_left(const T & v)
{
    _points[0][0] = v;
}

template<class T>
T& rect<T>::right()
{
    return _points[1][0];
}

template<class T>
const T& rect<T>::right() const
{
    return _points[1][0];
}

template<class T>
void rect<T>::set_right(const T & v)
{
    _points[1][0] = v;
}

template<class T>
T & rect<T>::bottom()
{
    return _points[0][1];
}

template<class T>
const T & rect<T>::bottom() const
{
    return _points[0][1];
}

template<class T>
void rect<T>::set_bottom(const T & v)
{
    _points[0][1] = v;
}

template<class T>
T & rect<T>::top()
{
    return _points[1][1];
}

template<class T>
const T & rect<T>::top() const
{
    return _points[1][1];
}

template<class T>
void rect<T>::set_top(const T & v)
{
    _points[1][1] = v;
}

template<class T>
point_type<T> rect<T>::bottom_left() const
{
    return _points[0];
}

template<class T>
void rect<T>::set_bottom_left(const point_type<T>& p)
{
    _points[0] = p;
}

template<class T>
point_type<T> rect<T>::bottom_right() const
{
    return { _points[1][0], _points[0][1] };
}

template<class T>
void rect<T>::set_bottom_right(const point_type<T>& p)
{
    _points[1][0] = p[0];
    _points[0][1] = p[1];
}

template<class T>
point_type<T> rect<T>::top_left() const
{
    return { _points[0][0], _points[1][1] };
}

template<class T>
void rect<T>::set_top_left(const point_type<T>& p)
{
    _points[0][0] = p[0];
    _points[1][1] = p[1];
}

template<class T>
point_type<T> rect<T>::top_right() const
{
    return _points[1];
}

template<class T>
void rect<T>::set_top_right(const point_type<T>& p)
{
    _points[1] = p;
}

template<class T>
point_type<T> rect<T>::center() const
{
    return (_points[0] + _points[1]) / static_cast<T>(2);
}

template<class T>
template<inclusion_mode mode>
bool rect<T>::contains(const point_type<T>& p) const
{
    auto normalized_this = normalized();

    if constexpr (mode == inclusion_mode::full)
    {
        return
            p[0] >= normalized_this._points[0][0] && p[0] <= normalized_this._points[1][0] &&
            p[1] >= normalized_this._points[0][1] && p[1] <= normalized_this._points[1][1];
    }
    if constexpr (mode == inclusion_mode::edgeless)
    {
        return
            p[0] > normalized_this._points[0][0] && p[0] < normalized_this._points[1][0] &&
            p[1] > normalized_this._points[0][1] && p[1] < normalized_this._points[1][1];
    }
    if constexpr (mode == inclusion_mode::edge_only)
    {
        return
            p[0] == normalized_this._points[0][0] && p[0] == normalized_this._points[1][0] &&
            p[1] == normalized_this._points[0][1] && p[1] == normalized_this._points[1][1];
    }
    return false;
}

template<class T>
template<inclusion_mode mode>
bool rect<T>::contains(const rect<T>& r) const
{
    return contains<mode>(r._points[0]) && contains<mode>(r._points[1]);
}

template<class T>
T & rect<T>::x()
{
    return _points[0][0];
}

template<class T>
const T & rect<T>::x() const
{
    return _points[0][0];
}

template<class T>
void rect<T>::set_x(const T & v)
{
    _points[0][0] = v;
}

template<class T>
T & rect<T>::y()
{
    return _points[0][1];
}

template<class T>
const T & rect<T>::y() const
{
    return _points[0][1];
}

template<class T>
void rect<T>::set_y(const T & v)
{
    _points[0][1] = v;
}

template<class T>
T rect<T>::width() const
{
    return _points[1][0] - _points[0][0];
}

template<class T>
void rect<T>::set_width(const T & w)
{
    _points[1][0] = _points[0][0] + w;
}

template<class T>
T rect<T>::height() const
{
    return _points[1][1] - _points[0][1];
}

template<class T>
void rect<T>::set_height(const T & h)
{
    _points[1][1] = _points[0][1] + h;
}

template<class T>
bool rect<T>::intersects(const rect<T>& r) const
{
    return contains(r._points[0]) || contains(r._points[1]);
}

template<class T>
std::optional<rect<T>> rect<T>::intersection(const rect<T>& r) const
{
    auto normalized_this = normalized();
    auto normalized_r = r.normalized();

    auto left_x = std::max(normalized_this._points[0][0], normalized_r._points[0][0]);
    auto right_x = std::min(normalized_this._points[1][0], normalized_r._points[1][0]);
    auto bottom_y = std::max(normalized_this._points[0][1], normalized_r._points[0][1]);
    auto top_y = std::min(normalized_this._points[1][1], normalized_r._points[1][1]);
    if (left_x <= right_x && bottom_y <= top_y)
    {
        return rect<T>(point_type<T>(left_x, bottom_y), point_type<T>(right_x, top_y));
    }
    return std::nullopt;
}

template<class T>
void rect<T>::normalize()
{
    auto left_x = std::min(_points[0][0], _points[1][0]);
    auto right_x = std::max(_points[0][0], _points[1][0]);
    auto top_y = std::max(_points[0][1], _points[1][1]);
    auto bottom_y = std::min(_points[0][1], _points[1][1]);
    _points[0][0] = left_x;
    _points[1][0] = right_x;
    _points[0][1] = bottom_y;
    _points[1][1] = top_y;
}

template<class T>
rect<T> rect<T>::normalized() const
{
    auto left_x = std::min(_points[0][0], _points[1][0]);
    auto right_x = std::max(_points[0][0], _points[1][0]);
    auto top_y = std::max(_points[0][1], _points[1][1]);
    auto bottom_y = std::min(_points[0][1], _points[1][1]);
    return rect<T>(point_type<T>(left_x, bottom_y), point_type<T>(right_x, top_y));
}

template<class T>
void rect<T>::translate(const T & dx, const T & dy)
{
    _points[0][0] += dx;
    _points[1][0] += dx;
    _points[0][1] += dy;
    _points[1][1] += dy;
}

template<class T>
void rect<T>::translate(const vector_type<T>& v)
{
    _points[0][0] += v[0];
    _points[1][0] += v[0];
    _points[0][1] += v[1];
    _points[1][1] += v[1];
}

template<class T>
rect<T> rect<T>::translated(const T & dx, const T & dy) const
{
    return rect<T>(point_type<T>(_points[0][0] + dx, _points[0][1] + dy), point_type<T>(_points[1][0] + dx, _points[1][1] + dy));
}

template<class T>
rect<T> rect<T>::translated(const vector_type<T>& v) const
{
    return rect<T>(point_type<T>(_points[0][0] + v[0], _points[0][1] + v[1]), point_type<T>(_points[1][0] + v[0], _points[1][1] + v[1]));
}

template<class T>
rect<T> rect<T>::united(const rect<T>& r) const
{
    auto left_x = std::min(_points[0][0], r._points[0][0]);
    auto right_x = std::max(_points[1][0], r._points[1][0]);
    auto bottom_y = std::max(_points[1][1], r._points[1][1]);
    auto top_y = std::min(_points[0][1], r._points[0][1]);
    return rect<T>(point_type<T>(left_x, bottom_y), point_type<T>(right_x, top_y));
}

template<class T>
bool rect<T>::operator==(const rect& other) const
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
bool rect<T>::operator!=(const rect& other) const
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