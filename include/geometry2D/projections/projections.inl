#pragma once

#include "projections.hpp"
#include "../primitives/line.hpp"
#include "../primitives/segment.hpp"
#include "../primitives/rect.hpp"
#include "../primitives/triangle.hpp"

NAMESPACE_GEOMETRY2D_BEGIN

template<class T>
point_type<T> projection(const point_type<T>& p, const line<T>& l1)
{
    //projected_point = line.p1 + t * line.vector
    //vector(projected_point,p) * line.vector = 0
    //t = - (vector(p,line.p1)*line.vector)/(line.vector.length)^2

    auto pp1 = l1[0] - p;
    auto p1p2 = l1[1] - l1[0];
    auto t = -(pp1 * p1p2) / (p1p2*p1p2);
    return l1[0] + t* p1p2;
}

template<class T>
point_type<T> projection(const point_type<T>& p, const segment<T>& s1)
{
    auto projected_point = projection(p, (line<T>)s1);

    auto min_x = std::min(s1[0][0], s1[1][0]);
    auto max_x = std::max(s1[0][0], s1[1][0]);
    auto min_y = std::min(s1[0][1], s1[1][1]);
    auto max_y = std::max(s1[0][1], s1[1][1]);

    if (projected_point[0] < min_x || projected_point[0] > max_x || projected_point[1] < min_y || projected_point[1] > max_y)
    {
        auto dist_to_p1 = (s1[0] - p).length();
        auto dist_to_p2 = (s1[1] - p).length();
        return dist_to_p1 < dist_to_p2 ? s1[0] : s1[1];
    }

    return projected_point;
}

template<class T>
point_type<T> projection(const point_type<T>& p, const rect<T>& r1)
{
    if(r1.contains(p))
    {
        return p;
    }

    std::array<point_type<T>, 4> projections_on_boundary;
    projections_on_boundary[0] = projection(p, segment<T>(r1.bottom_left(), r1.bottom_right()));
    projections_on_boundary[1] = projection(p, segment<T>(r1.bottom_right(), r1.top_right()));
    projections_on_boundary[2] = projection(p, segment<T>(r1.top_right(), r1.top_left()));
    projections_on_boundary[3] = projection(p, segment<T>(r1.top_left(), r1.bottom_left()));

    auto closest = std::min_element(projections_on_boundary.begin(), projections_on_boundary.end(), [&p](const point_type<T>& bp) {
        return (bp - p).length();
    });

    return *closest;
}

template<class T>
point_type<T> projection(const point_type<T>& p, const triangle<T>& t1)
{
    if (t1.contains(p))
    {
        return p;
    }

    std::array<point_type<T>, 3> projections_on_boundary;
    projections_on_boundary[0] = projection(p, segment<T>(t1[0], t1[1]));
    projections_on_boundary[1] = projection(p, segment<T>(t1[1], t1[2]));
    projections_on_boundary[2] = projection(p, segment<T>(t1[2], t1[0]));

    auto closest = std::min_element(projections_on_boundary.begin(), projections_on_boundary.end(), [&p](const point_type<T>& bp) {
        return (bp - p).length();
    });

    return *closest;
}

NAMESPACE_GEOMETRY2D_END