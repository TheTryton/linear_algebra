#pragma once

#include "../geometry_common.hpp"

NAMESPACE_GEOMETRY_BEGIN

NAMESPACE_INTERSECTIONS_BEGIN

namespace detail
{
    template<class T, size_t D, class SD>
    struct space_space_intersection_type_impl
    {
        using space_space_intersection_type = geometry::point_type<T, D>;
    };

    template<class T, size_t D, size_t... SD>
    struct space_space_intersection_type_impl<T, D, std::index_sequence<SD...>>
    {
        using space_space_intersection_type = std::variant<geometry::point_type<T, D>, geometry::space<T, SD + 1, D>...>;
    };
}

template<class T, size_t D, size_t SD1, size_t SD2>
using space_space_intersection = typename detail::space_space_intersection_type_impl <
    T,
    D,
    decltype(std::make_index_sequence<
        SD1 <= SD2 ? SD1 : SD2
    >())
>::space_space_intersection_type;


template<class T, size_t D>
using line_line_intersection = space_space_intersection<T, D, 1, 1>;

template<class T, size_t D>
using line_plane_intersection = space_space_intersection<T, D, 1, 2>;

template<class T, size_t D>
using plane_plane_intersection = space_space_intersection<T, D, 2, 2>;

//line-line
template<class T, size_t D>
std::optional<line_line_intersection<T, D>> intersection(const space<T, 1, D>& s1, const space<T, 1, D>& s2);

//line-plane
template<class T, size_t D>
std::optional<line_plane_intersection<T, D>> intersection(const space<T, 1, D>& s1, const space<T, 2, D>& s2);

template<class T, size_t D>
std::optional<line_plane_intersection<T, D>> intersection(const space<T, 2, D>& s1, const space<T, 1, D>& s2);

//plane-plane
template<class T, size_t D>
std::optional<plane_plane_intersection<T, D>> intersection(const space<T, 2, D>& s1, const space<T, 2, D>& s2);

//space-space

template<class T, size_t D, size_t SD1, size_t SD2>
std::optional<space_space_intersection<T, D, SD1, SD2>> intersection(const space<T, SD1, D>& s1, const space<T, SD2, D>& s2);

NAMESPACE_INTERSECTIONS_END

NAMESPACE_GEOMETRY_END