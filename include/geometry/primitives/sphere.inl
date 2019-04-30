#pragma once

#include "sphere.hpp"
#include "../intersections/intersections.hpp"
#include "../intersections/intersections.inl"

NAMESPACE_GEOMETRY_BEGIN

template<class T, size_t D>
sphere<T, D>::sphere():
    _radius(static_cast<T>(0))
{
}

template<class T, size_t D>
sphere<T, D>::sphere(const sphere<T, D>& other) :
    _center(other._center),
    _radius(other._radius)
{
}

template<class T, size_t D>
sphere<T, D>::sphere(sphere<T, D>&& other) :
    _center(std::move(other._center)),
    _radius(std::move(other._radius))
{
}

template<class T, size_t D>
sphere<T, D>& sphere<T, D>::operator=(const sphere<T, D>& other)
{
    _center = other._center;
    _radius = other._radius;

    return *this;
}

template<class T, size_t D>
sphere<T, D>& sphere<T, D>::operator=(sphere<T, D>&& other)
{
    _center = std::move(other._center);
    _radius = std::move(other._radius);

    return *this;
}

template<class T, size_t D>
template<class TO, typename>
sphere<T, D>::sphere(const sphere<TO, D>& other) :
    _center(other._center),
    _radius(static_cast<T>(other._radius))
{
}

template<class T, size_t D>
template<class TO, typename>
sphere<T, D>::sphere(sphere<TO, D>&& other) :
    _center(other._center),
    _radius(static_cast<T>(other._radius))
{
}

template<class T, size_t D>
template<class TO, typename>
sphere<T, D>& sphere<T, D>::operator=(const sphere<TO, D>& other)
{
    _center = other._center;
    _radius = static_cast<T>(other._radius);

    return *this;
}

template<class T, size_t D>
template<class TO, typename>
sphere<T, D>& sphere<T, D>::operator=(sphere<TO, D>&& other)
{
    _center = other._center;
    _radius = static_cast<T>(other._radius);

    return *this;
}

template<class T, size_t D>
template<class TO1, class TO2, size_t DO, typename>
sphere<T, D>::sphere(const point_type<TO1, D>& center, const TO2& radius)
{
    _center = static_cast<point_type<T, D>>(center);
    _radius = static_cast<T>(radius);
}

template<class T, size_t D>
std::optional<std::pair<point_type<T, D>, T>> get_sphere_center_and_radius(std::array<point_type<T, D>, D + 1>&& points)
{
    LINEAR_ALGEBRA::matrix<T, D + 1, D + 1> equation_system;

    for (size_t row = 0; row < D + 1; row++)
    {
        for (size_t column = 0; column < D; column++)
        {
            equation_system[row][column] = std::move(points[row][column]);
        }
        equation_system[row][D] = static_cast<T>(1);
    }

    point_type<T, D + 1> constant_terms;

    for (size_t d = 0; d < D + 1; d++)
    {
        constant_terms[d] = static_cast<T>(-1) * points[d].length_sqr();
    }

    auto solution = equation_system.inverted();
    if(!solution)
    {
        return std::nullopt;
    }
    else
    {
        auto result = *solution * constant_terms;

        point_type<T, D> center;
        T radius = static_cast<T>(0);

        for (size_t d = 0; d < D; d++)
        {
            center[d] = result[d] / static_cast<T>(-2);
            radius += result[d] * result[d];
        }

        radius -= static_cast<T>(4) * result[D];

        radius = LINEAR_ALGEBRA::functions_implementation<T>::sqrt(radius) / static_cast<T>(2);

        return std::make_pair(center, radius);
    }
}

template<class T, size_t D>
template<class ...TOS, typename>
sphere<T, D>::sphere(const TOS&... vs)
{
    std::array<T, (D + 1)*D> coords = { static_cast<T>(vs)... };

    std::array<point_type<T, D>, D + 1> points;

    for (size_t p = 0; p < D + 1; p++)
    {
        for (size_t c = 0; c < D; c++)
        {
            points[p][c] = std::move(coords[p*D + c]);
        }
    }

    auto result = get_sphere_center_and_radius(std::move(points));

    if(!result)
    {
        _center = point_type<T, D>();
        _radius = static_cast<T>(0);
    }
    else
    {
        _center = result->first;
        _radius = result->second;
    }
}

template<class T, size_t D>
template<class ...TOS, typename>
sphere<T, D>::sphere(TOS&& ...vs)
{
    std::array<T, (D + 1)*D> coords = { static_cast<T>(vs)... };

    std::array<point_type<T, D>, D + 1> points;

    for (size_t p = 0; p < D + 1; p++)
    {
        for (size_t c = 0; c < D; c++)
        {
            points[p][c] = std::move(coords[p*D + c]);
        }
    }

    auto result = get_sphere_center_and_radius(std::move(points));

    if (!result)
    {
        _center = point_type<T, D>();
        _radius = static_cast<T>(0);
    }
    else
    {
        _center = result->first;
        _radius = result->second;
    }
}

template<class T, size_t D>
template<class ...TOS, size_t ...DOS, typename>
sphere<T, D>::sphere(const point_type<TOS, DOS>& ...vs)
{
    std::array<point_type<T, D>, D + 1> points = { static_cast<point_type<T, D>>(vs)... };

    auto result = get_sphere_center_and_radius(std::move(points));

    if (!result)
    {
        _center = point_type<T, D>();
        _radius = static_cast<T>(0);
    }
    else
    {
        _center = result->first;
        _radius = result->second;
    }
}

template<class T, size_t D>
template<class ...TOS, size_t ...DOS, typename>
sphere<T, D>::sphere(point_type<TOS, DOS>&& ...vs)
{
    std::array<point_type<T, D>, D + 1> points = { static_cast<point_type<T, D>>(vs)... };

    auto result = get_sphere_center_and_radius(std::move(points));

    if (!result)
    {
        _center = point_type<T, D>();
        _radius = static_cast<T>(0);
    }
    else
    {
        _center = result->first;
        _radius = result->second;
    }
}

template<class T, size_t D>
template<class TO, typename>
sphere<T, D>::sphere(std::initializer_list<TO> init_list)
{
    if (init_list.size() != D * (D + 1))
    {
        _center = point_type<T, D>();
        _radius = static_cast<T>(0);
        return;
    }

    std::array<T, (D + 1)*D> coords;

    std::transform(init_list.begin(), init_list.end(), coords.begin(), [](const TO& v) {
        return static_cast<T>(v);
    });

    std::array<point_type<T, D>, D + 1> points;

    for (size_t p = 0; p < D + 1; p++)
    {
        for (size_t c = 0; c < D; c++)
        {
            points[p][c] = std::move(coords[p*D + c]);
        }
    }

    auto result = get_sphere_center_and_radius(std::move(points));

    if (!result)
    {
        _center = point_type<T, D>();
        _radius = static_cast<T>(0);
    }
    else
    {
        _center = result->first;
        _radius = result->second;
    }
}

template<class T, size_t D>
template<class TO, size_t DO, typename>
sphere<T, D>::sphere(std::initializer_list<point_type<TO, DO>> init_list)
{
    if (init_list.size() != D + 1)
    {
        _center = point_type<T, D>();
        _radius = static_cast<T>(0);
        return;
    }

    std::array<point_type<T, D>, D + 1> points;

    std::transform(init_list.begin(), init_list.end(), points.begin(), [](const point_type<TO, DO>& v) {
        return static_cast<point_type<T, D>>(v);
    });

    auto result = get_sphere_center_and_radius(std::move(points));

    if (!result)
    {
        _center = point_type<T, D>();
        _radius = static_cast<T>(0);
    }
    else
    {
        _center = result->first;
        _radius = result->second;
    }
}

template<class T, size_t D>
template<class TO, typename>
sphere<T, D>& sphere<T, D>::operator=(std::initializer_list<TO> init_list)
{
    if (init_list.size() != D * (D + 1))
    {
        _center = point_type<T, D>();
        _radius = static_cast<T>(0);
        return *this;
    }

    std::array<T, (D + 1)*D> coords;

    std::transform(init_list.begin(), init_list.end(), coords.begin(), [](const TO& v) {
        return static_cast<T>(v);
    });

    std::array<point_type<T, D>, D + 1> points;

    for (size_t p = 0; p < D + 1; p++)
    {
        for (size_t c = 0; c < D; c++)
        {
            points[p][c] = std::move(coords[p*D + c]);
        }
    }

    auto result = get_sphere_center_and_radius(std::move(points));

    if (!result)
    {
        _center = point_type<T, D>();
        _radius = static_cast<T>(0);
    }
    else
    {
        _center = result->first;
        _radius = result->second;
    }
}

template<class T, size_t D>
template<class TO, size_t DO, typename>
sphere<T, D>& sphere<T, D>::operator=(std::initializer_list<point_type<TO, DO>> init_list)
{
    if (init_list.size() != D + 1)
    {
        _center = point_type<T, D>();
        _radius = static_cast<T>(0);
        return *this;
    }

    std::array<point_type<T, D>, D + 1> points;

    std::transform(init_list.begin(), init_list.end(), points.begin(), [](const point_type<TO, DO>& v) {
        return static_cast<point_type<T, D>>(v);
    });

    auto result = get_sphere_center_and_radius(std::move(points));

    if (!result)
    {
        _center = point_type<T, D>();
        _radius = static_cast<T>(0);
    }
    else
    {
        _center = result->first;
        _radius = result->second;
    }
}

template<class T, size_t D>
point_type<T, D>& sphere<T, D>::center()
{
    return _center;
}

template<class T, size_t D>
const point_type<T, D>& sphere<T, D>::center() const
{
    return _center;
}

template<class T, size_t D>
void sphere<T, D>::set_center(const point_type<T, D>& center)
{
    _center = center;
}

template<class T, size_t D>
T& sphere<T, D>::radius()
{
    return _radius;
}

template<class T, size_t D>
const T& sphere<T, D>::radius() const
{
    return _radius;
}

template<class T, size_t D>
void sphere<T, D>::set_radius(const T& radius)
{
    _radius = radius;
}

template<class T, size_t D>
template<inclusion_mode mode>
bool sphere<T, D>::contains(const point_type<T, D>& p) const
{
    if constexpr (mode == inclusion_mode::full)
    {
        return (p - _center).length_sqr() <= _radius * _radius;
    }
    if constexpr (mode == inclusion_mode::edgeless)
    {
        return (p - _center).length_sqr() < _radius * _radius;
    }
    if constexpr (mode == inclusion_mode::edge_only)
    {
        return equal((p - _center).length_sqr(), _radius * _radius);
    }

    return false;
}

NAMESPACE_GEOMETRY_IO_BEGIN

template<class TO, size_t DO>
std::ostream& operator<<(std::ostream& os, const sphere<TO, DO>& s)
{
	using linear_algebra::linear_algebra_io::operator<<;

    os << DO << "-dimensional sphere in " << DO << "-dimensional vector space:" << std::endl;
    os << "{" << std::endl;
    os << "\tcenter = " << s._center << std::endl;
    os << "\tradius = " << s._radius << std::endl;
    os << "}";
    return os;
}

NAMESPACE_GEOMETRY_IO_END

NAMESPACE_GEOMETRY_END