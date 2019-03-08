#pragma once

#include "space.hpp"
#include "simplex.hpp"

NAMESPACE_GEOMETRY_BEGIN

template<class T, size_t SD, size_t D>
space<T, SD, D>::space()
{
    std::fill(_points.begin(), _points.end(), point_type<T, D>());
}

template<class T, size_t SD, size_t D>
space<T, SD, D>::space(const space<T, SD, D>& other)
{
    std::copy(other._points.begin(), other._points.end(), _points.begin());
}

template<class T, size_t SD, size_t D>
space<T, SD, D>::space(space<T, SD, D>&& other)
{
    std::move(other._points.begin(), other._points.end(), _points.begin());
}

template<class T, size_t SD, size_t D>
space<T, SD, D>& space<T, SD, D>::operator=(const space<T, SD, D>& other)
{
    std::copy(other._points.begin(), other._points.end(), _points.begin());

    return *this;
}

template<class T, size_t SD, size_t D>
space<T, SD, D>& space<T, SD, D>::operator=(space<T, SD, D>&& other)
{
    std::move(other._points.begin(), other._points.end(), _points.begin());

    return *this;
}

template<class T, size_t SD, size_t D>
template<class TO, typename>
space<T, SD, D>::space(const space<TO, SD, D>& other)
{
    std::copy(other._points.begin(), other._points.end(), _points.begin());
}

template<class T, size_t SD, size_t D>
template<class TO, typename>
space<T, SD, D>::space(space<TO, SD, D>&& other)
{
    std::move(other._points.begin(), other._points.end(), _points.begin());
}

template<class T, size_t SD, size_t D>
template<class TO, typename>
space<T, SD, D>& space<T, SD, D>::operator=(const space<TO, SD, D>& other)
{
    std::copy(other._points.begin(), other._points.end(), _points.begin());

    return *this;
}

template<class T, size_t SD, size_t D>
template<class TO, typename>
space<T, SD, D>& space<T, SD, D>::operator=(space<TO, SD, D>&& other)
{
    std::move(other._points.begin(), other._points.end(), _points.begin());

    return *this;
}

template<class T, size_t SD, size_t D>
template<class TO, size_t DO, typename>
space<T, SD, D>::space(const space<TO, SD, DO>& other)
{
    std::copy(other._points.begin(), other._points.end(), _points.begin());
}

template<class T, size_t SD, size_t D>
template<class TO, size_t DO, typename>
space<T, SD, D>::space(space<TO, SD, DO>&& other)
{
    std::move(other._points.begin(), other._points.end(), _points.begin());
}

template<class T, size_t SD, size_t D>
template<class TO, size_t DO, typename>
space<T, SD, D>& space<T, SD, D>::operator=(const space<TO, SD, DO>& other)
{
    std::copy(other._points.begin(), other._points.end(), _points.begin());

    return *this;
}

template<class T, size_t SD, size_t D>
template<class TO, size_t DO, typename>
space<T, SD, D>& space<T, SD, D>::operator=(space<TO, SD, DO>&& other)
{
    std::move(other._points.begin(), other._points.end(), _points.begin());

    return *this;
}

template<class T, size_t SD, size_t D>
template<class ...TOS, typename>
space<T, SD, D>::space(const TOS&... vs)
{
    std::array<T, (SD + 1)*D> coords = { static_cast<T>(vs)... };

    for (size_t p = 0; p < SD + 1; p++)
    {
        for (size_t c = 0; c < D; c++)
        {
            _points[p][c] = coords[p*D + c];
        }
    }
}

template<class T, size_t SD, size_t D>
template<class ...TOS, typename>
space<T, SD, D>::space(TOS&&... vs)
{
    std::array<T, (SD + 1)*D> coords = { static_cast<T>(vs)... };

    for (size_t p = 0; p < SD + 1; p++)
    {
        for (size_t c = 0; c < D; c++)
        {
            _points[p][c] = std::move(coords[p*D + c]);
        }
    }
}

template<class T, size_t SD, size_t D>
template<class ...TOS, size_t... DOS, typename>
space<T, SD, D>::space(const point_type<TOS, DOS>&... vs)
{
    std::array<point_type<T, D>, SD + 1> points = { static_cast<point_type<T, D>>(vs)... };

    for (size_t p = 0; p < SD + 1; p++)
    {
        _points[p] = points[p];
    }
}

template<class T, size_t SD, size_t D>
template<class ...TOS, size_t... DOS, typename>
space<T, SD, D>::space(point_type<TOS, DOS>&&... vs)
{
    std::array<point_type<T, D>, SD + 1> points = { static_cast<point_type<T, D>>(vs)... };

    for (size_t p = 0; p < SD + 1; p++)
    {
        _points[p] = std::move(points[p]);
    }
}

template<class T, size_t SD, size_t D>
template<class TO, typename>
space<T, SD, D>::space(std::initializer_list<TO> init_list) :
    space()
{
    if (init_list.size() < (SD + 1)*D)
    {
        return;
    }

    auto b = init_list.begin();

    size_t pt = 0;
    size_t coord = 0;
    for (; pt < _points.size(); pt++, coord = 0)
    {
        auto& p = _points[pt];
        for (; coord < p.size(); coord++)
        {
            p[coord] = static_cast<T>(*b++);
            if (b == init_list.end())
                break;
        }
        if (b == init_list.end())
            break;
    }

    for (; pt < _points.size(); pt++, coord = 0)
    {
        auto& p = _points[pt];
        for (; coord < p.size(); coord++)
        {
            p[coord] = static_cast<T>(0);
        }
    }
}

template<class T, size_t SD, size_t D>
template<class TO, size_t DO, typename>
space<T, SD, D>::space(std::initializer_list<point_type<TO, DO>> init_list) :
    space()
{
    if (init_list.size() < SD + 1)
    {
        return;
    }

    auto b = init_list.begin();

    size_t pt = 0;
    for (; pt < _points.size(); pt++)
    {
        if (b == init_list.end())
            break;
        _points[pt] = static_cast<point_type<T, D>>(*b++);
    }

    for (; pt < _points.size(); pt++)
    {
        _points[pt] = point_type<T, D>();
    }
}

template<class T, size_t SD, size_t D>
template<class TO, typename>
space<T, SD, D>& space<T, SD, D>::operator=(std::initializer_list<TO> init_list)
{
    if (init_list.size() < (SD + 1)*D)
    {
        return;
    }

    auto b = init_list.begin();

    for (auto& pt : _points)
    {
        if (b == init_list.end())
            return *this;
        for (auto& coord : pt)
        {
            if (b == init_list.end())
                return *this;
            coord = static_cast<T>(*b++);
        }
    }

    return *this;
}

template<class T, size_t SD, size_t D>
template<class TO, size_t DO, typename>
space<T, SD, D>& space<T, SD, D>::operator=(std::initializer_list<point_type<TO, DO>> init_list)
{
    if (init_list.size() < SD + 1)
    {
        return;
    }

    auto b = init_list.begin();

    for (auto& pt : _points)
    {
        if (b == init_list.end())
            return *this;
        pt = static_cast<point_type<T, D>>(*b++);
    }

    return *this;
}

template<class T, size_t SD, size_t D>
space<T, SD, D>::space(const simplex<T, SD, D>& other)
{
    std::copy(other._points.begin(), other._points.end(), _points.begin());
}

template<class T, size_t SD, size_t D>
space<T, SD, D>::space(simplex<T, SD, D>&& other)
{
    std::move(other._points.begin(), other._points.end(), _points.begin());
}

template<class T, size_t SD, size_t D>
space<T, SD, D>& space<T, SD, D>::operator=(const simplex<T, SD, D>& other)
{
    std::copy(other._points.begin(), other._points.end(), _points.begin());

    return *this;
}

template<class T, size_t SD, size_t D>
space<T, SD, D>& space<T, SD, D>::operator=(simplex<T, SD, D>&& other)
{
    std::move(other._points.begin(), other._points.end(), _points.begin());

    return *this;
}

template<class T, size_t SD, size_t D>
template<class TO>
space<T, SD, D>::space(const simplex<TO, SD, D>& other)
{
    std::copy(other._points.begin(), other._points.end(), _points.begin());
}

template<class T, size_t SD, size_t D>
template<class TO>
space<T, SD, D>::space(simplex<TO, SD, D>&& other)
{
    std::move(other._points.begin(), other._points.end(), _points.begin());
}

template<class T, size_t SD, size_t D>
template<class TO>
space<T, SD, D>& space<T, SD, D>::operator=(const simplex<TO, SD, D>& other)
{
    std::copy(other._points.begin(), other._points.end(), _points.begin());

    return *this;
}

template<class T, size_t SD, size_t D>
template<class TO>
space<T, SD, D>& space<T, SD, D>::operator=(simplex<TO, SD, D>&& other)
{
    std::move(other._points.begin(), other._points.end(), _points.begin());

    return *this;
}

template<class T, size_t SD, size_t D>
template<class TO, size_t DO>
space<T, SD, D>::space(const simplex<TO, SD, DO>& other)
{
    std::copy(other._points.begin(), other._points.end(), _points.begin());
}

template<class T, size_t SD, size_t D>
template<class TO, size_t DO>
space<T, SD, D>::space(simplex<TO, SD, DO>&& other)
{
    std::move(other._points.begin(), other._points.end(), _points.begin());
}

template<class T, size_t SD, size_t D>
template<class TO, size_t DO>
space<T, SD, D>& space<T, SD, D>::operator=(const simplex<TO, SD, DO>& other)
{
    std::copy(other._points.begin(), other._points.end(), _points.begin());

    return *this;
}

template<class T, size_t SD, size_t D>
template<class TO, size_t DO>
space<T, SD, D>& space<T, SD, D>::operator=(simplex<TO, SD, DO>&& other)
{
    std::move(other._points.begin(), other._points.end(), _points.begin());

    return *this;
}

template<class T, size_t SD, size_t D>
point_type<T, D>* space<T, SD, D>::data()
{
    return _points.data();
}

template<class T, size_t SD, size_t D>
const point_type<T, D>* space<T, SD, D>::data() const
{
    return _points.data();
}

template<class T, size_t SD, size_t D>
constexpr size_t space<T, SD, D>::size()
{
    return SD + 1;
}

template<class T, size_t SD, size_t D>
point_type<T, D>& space<T, SD, D>::operator[](size_t i)
{
    return _points[i];
}

template<class T, size_t SD, size_t D>
const point_type<T, D>& space<T, SD, D>::operator[](size_t i) const
{
    return _points[i];
}

template<class T, size_t SD, size_t D>
auto space<T, SD, D>::begin()
{
    return _points.begin();
}

template<class T, size_t SD, size_t D>
auto space<T, SD, D>::begin() const
{
    return _points.begin();
}

template<class T, size_t SD, size_t D>
auto space<T, SD, D>::end()
{
    return _points.end();
}

template<class T, size_t SD, size_t D>
auto space<T, SD, D>::end() const
{
    return _points.end();
}

template<class T, size_t SD, size_t D>
point_type<T, D>& space<T, SD, D>::p1()
{
    return _points[0];
}

template<class T, size_t SD, size_t D>
const point_type<T, D>& space<T, SD, D>::p1() const
{
    return _points[0];
}

template<class T, size_t SD, size_t D>
void space<T, SD, D>::set_p1(const point_type<T, D>& p)
{
    _points[0] = p;
}

template<class T, size_t SD, size_t D>
template<typename>
point_type<T, D>& space<T, SD, D>::p2()
{
    return _points[1];
}

template<class T, size_t SD, size_t D>
template<typename>
const point_type<T, D>& space<T, SD, D>::p2() const
{
    return _points[1];
}

template<class T, size_t SD, size_t D>
template<typename>
void space<T, SD, D>::set_p2(const point_type<T, D>& p)
{
    _points[1] = p;
}

template<class T, size_t SD, size_t D>
template<typename>
point_type<T, D>& space<T, SD, D>::p3()
{
    return _points[2];
}

template<class T, size_t SD, size_t D>
template<typename>
const point_type<T, D>& space<T, SD, D>::p3() const
{
    return _points[2];
}

template<class T, size_t SD, size_t D>
template<typename>
void space<T, SD, D>::set_p3(const point_type<T, D>& p)
{
    _points[2] = p;
}

template<class T, size_t SD, size_t D>
template<typename>
point_type<T, D>& space<T, SD, D>::p4()
{
    return _points[3];
}

template<class T, size_t SD, size_t D>
template<typename>
const point_type<T, D>& space<T, SD, D>::p4() const
{
    return _points[3];
}

template<class T, size_t SD, size_t D>
template<typename>
void space<T, SD, D>::set_p4(const point_type<T, D>& p)
{
    _points[3] = p;
}

template<class T, size_t SD, size_t D>
template<typename>
point_type<T, D>& space<T, SD, D>::p5()
{
    return _points[4];
}

template<class T, size_t SD, size_t D>
template<typename>
const point_type<T, D>& space<T, SD, D>::p5() const
{
    return _points[4];
}

template<class T, size_t SD, size_t D>
template<typename>
void space<T, SD, D>::set_p5(const point_type<T, D>& p)
{
    _points[4] = p;
}

NAMESPACE_GEOMETRY_IO_BEGIN

template<class TO, size_t SDO, size_t DO>
std::ostream& operator<<(std::ostream& os, const space<TO, SDO, DO>& s)
{
    os << SDO << "-dimensional space in " << DO << "-dimensional vector space:" << std::endl;
    os << "{" << std::endl;
    for (size_t i = 0; i < SDO; i++)
    {
        os << "\tpt" << i << " = " << s._points[i] << ", " << std::endl;
    }
    os << "\tpt" << SDO << " = " << s._points[SDO] << std::endl;
    os << "}";
    return os;
}

NAMESPACE_GEOMETRY_IO_END

NAMESPACE_GEOMETRY_END