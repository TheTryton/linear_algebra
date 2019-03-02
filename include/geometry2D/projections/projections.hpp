#pragma once

#include "../geometry2D_common.hpp"

NAMESPACE_GEOMETRY2D_BEGIN

//point on line, point on segment
template<class T>
point_type<T> projection(const point_type<T>& p, const line<T>& l1);
template<class T>
point_type<T> projection(const point_type<T>& p, const segment<T>& s1);

//point on rect, point on triangle (closest point contained within shape to specified point)
template<class T>
point_type<T> projection(const point_type<T>& p, const rect<T>& r1);
template<class T>
point_type<T> projection(const point_type<T>& p, const triangle<T>& t1);

NAMESPACE_GEOMETRY2D_END