#pragma once

#include "../geometry2D_common.hpp"

NAMESPACE_GEOMETRY2D_BEGIN

//line - line, line - segment, segment - segment
template<class T>
std::optional<std::variant<point_type<T>, line<T>>> intersection(const line<T>& l1, const line<T>& l2);
template<class T>
std::optional<std::variant<point_type<T>, segment<T>>> intersection(const segment<T>& s1, const line<T>& l2);
template<class T>
std::optional<std::variant<point_type<T>, segment<T>>> intersection(const line<T>& l1, const segment<T>& s2);
template<class T>
std::optional<std::variant<point_type<T>, segment<T>>> intersection(const segment<T>& s1, const segment<T>& s2);

//rect - line, rect - segment
template<class T>
std::optional<std::variant<point_type<T>, std::array<point_type<T>, 2>, segment<T>>> intersection(const rect<T>& r1, const line<T>& l2);
template<class T>
std::optional<std::variant<point_type<T>, std::array<point_type<T>, 2>, segment<T>>> intersection(const rect<T>& r1, const segment<T>& s2);

//rect - rect
template<class T>
std::optional<std::variant<rect<T>, point_type<T>, segment<T>>> intersection(const rect<T>& r1, const rect<T>& r2);

NAMESPACE_GEOMETRY2D_END