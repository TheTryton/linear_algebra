#pragma once

#include "../geometry_common.hpp"

NAMESPACE_GEOMETRY_BEGIN

NAMESPACE_PROJECTIONS_BEGIN

//point on space
template<class T, size_t SD, size_t D>
std::optional<point_type<T, D>> project(const space<T, SD, D>& s, const point_type<T, D>& p);

//point on simplex
template<class T, size_t SD, size_t D>
std::optional<point_type<T, D>> closest_point(const simplex<T, SD, D>& s, const point_type<T, D>& p);

NAMESPACE_PROJECTIONS_END

NAMESPACE_GEOMETRY_END