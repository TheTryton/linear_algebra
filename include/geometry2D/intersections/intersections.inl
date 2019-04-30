#pragma once

#include "intersections.hpp"
#include "../primitives/line.hpp"
#include "../primitives/segment.hpp"
#include "../primitives/rect.hpp"

NAMESPACE_GEOMETRY2D_BEGIN

template<class T>
std::optional<std::variant<point_type<T>, line<T>>> intersection(const line<T>& l1, const line<T>& l2)
{
    auto p1p2 = l1[1] - l1[0];
    auto p4p3 = l2[0] - l2[1];
    auto p1p3 = l2[0] - l1[0];

    //check if both lines are declared with valid points (i.e line points are not the same)
    if (p1p2.length_sqr() <= std::numeric_limits<T>::epsilon() || p4p3.length_sqr() <= std::numeric_limits<T>::epsilon())
    {
        return std::nullopt;
    }

    //check if parallel
    auto w = p1p2[0] * p4p3[1] - p1p2[1] * p4p3[0];
    if (std::abs(w) <= std::numeric_limits<T>::epsilon())
    {
        //check if lines are the same
        auto test_v = p1p2[1] * p1p3[0] - p1p2[0] * p1p3[1];
        if (std::abs(test_v) <= std::numeric_limits<T>::epsilon())
        {
            return l1;
        }
        return std::nullopt;
    }

    auto wt = p1p3[0] * p4p3[1] - p1p3[1] * p4p3[0];

    auto t = wt / w;

    return l1[0] + p1p2 * t;
}

template<class T>
std::optional<std::variant<point_type<T>, segment<T>>> intersection(const segment<T>& s1, const line<T>& l2)
{
    auto line_line = intersection(static_cast<line<T>>(s1), l2);
    //if line-line intersection returned no point of intersection then segment will not have a point of intersection with line
    if (!line_line)
    {
        return std::nullopt;
    }
    //if lines are the same then segment will be mathematical intersection of segment and line shape
    if ((*line_line).index() == 1)
    {
        return s1;
    }

    auto min_x = std::min(s1[0][0], s1[1][0]);
    auto max_x = std::max(s1[0][0], s1[1][0]);
    auto min_y = std::min(s1[0][1], s1[1][1]);
    auto max_y = std::max(s1[0][1], s1[1][1]);

    const auto& point = std::get<0>(*line_line);

    //check if contained within s1
    if (point[0] >= min_x && point[0] <= max_x && point[1] >= min_y && point[1] <= max_y)
    {
        return point;
    }

    return std::nullopt;
}

template<class T>
std::optional<std::variant<point_type<T>, segment<T>>> intersection(const line<T>& l1, const segment<T>& s2)
{
    return intersection(s2, l1);
}

template<class T>
std::optional<std::variant<point_type<T>, segment<T>>> intersection(const segment<T>& s1, const segment<T>& s2)
{
    auto line_line = intersection(static_cast<line<T>>(s1), static_cast<line<T>>(s2));
    //if line-line intersection returned no point of intersection then segment will not have a point of intersection with line
    if (!line_line)
    {
        return std::nullopt;
    }
    //if parallel then return mathematical intersection of two segments
    if ((*line_line).index() == 1)
    {
        auto p1p2 = s1[1] - s1[0];

        auto min_x1 = std::min(s1[0][0], s1[1][0]);
        auto max_x1 = std::max(s1[0][0], s1[1][0]);
        auto min_x2 = std::min(s2[0][0], s2[1][0]);
        auto max_x2 = std::max(s2[0][0], s2[1][0]);

        auto min_x_s = std::max(min_x1, min_x2);
        auto max_x_s = std::min(max_x1, max_x2);

        auto min_y1 = std::min(s1[0][1], s1[1][1]);
        auto max_y1 = std::max(s1[0][1], s1[1][1]);
        auto min_y2 = std::min(s2[0][1], s2[1][1]);
        auto max_y2 = std::max(s2[0][1], s2[1][1]);

        auto min_y_s = std::max(min_y1, min_y2);
        auto max_y_s = std::min(max_y1, max_y2);

        //parallel segments touching with ends
        if(std::abs(min_x_s - max_x_s) <= std::numeric_limits<T>::epsilon() && std::abs(min_y_s - max_y_s) <= std::numeric_limits<T>::epsilon())
        {
            return point_type<T>(min_x_s, min_y_s);
        }

        //first quarter
        if (p1p2[0] >= 0 && p1p2[1] >= 0)
        {
            return segment<T>(min_x_s, min_y_s, max_x_s, max_y_s);
        }
        //second quarter
        if (p1p2[0] < 0 && p1p2[1] > 0)
        {
            return segment<T>(max_x_s, min_y_s, min_x_s, max_y_s);
        }
        //third quarter
        if (p1p2[0] <= 0 && p1p2[1] <= 0)
        {
            return segment<T>(max_x_s, max_y_s, min_x_s, min_y_s);
        }
        //fourth quarter
        return segment<T>(min_x_s, max_y_s, max_x_s, min_y_s);
    }

    auto min_x = std::min(s1[0][0], s1[1][0]);
    auto max_x = std::max(s1[0][0], s1[1][0]);
    auto min_y = std::min(s1[0][1], s1[1][1]);
    auto max_y = std::max(s1[0][1], s1[1][1]);

    const auto& point = std::get<0>(*line_line);

    //check if contained within s1
    if (point[0] >= min_x && point[0] <= max_x && point[1] >= min_y && point[1] <= max_y)
    {
        min_x = std::min(s2[0][0], s2[1][0]);
        max_x = std::max(s2[0][0], s2[1][0]);
        min_y = std::min(s2[0][1], s2[1][1]);
        max_y = std::max(s2[0][1], s2[1][1]);
        //check if contained within s2
        if (point[0] >= min_x && point[0] <= max_x && point[1] >= min_y && point[1] <= max_y)
        {
            return point;
        }
    }

    return std::nullopt;
}

template<class T>
std::optional<std::variant<point_type<T>, std::array<point_type<T>, 2>, segment<T>>> intersection(const rect<T>& r1, const line<T>& l2)
{
    std::array<std::optional<std::variant<point_type<T>, segment<T>>>, 4> lr_i =
    {
        intersection(segment<T>(r1.top_left(), r1.top_right()), l2),
        intersection(segment<T>(r1.top_right(), r1.bottom_right()), l2),
        intersection(segment<T>(r1.bottom_right(), r1.bottom_left()), l2),
        intersection(segment<T>(r1.bottom_left(), r1.top_left()), l2),
    };

    std::set<point_type<T>> result_points;
    for (auto& ll_i : lr_i)
    {
        if (ll_i)
        {
            //if there is a point of intersection between line l and segment made from edges then add this point to the final output array
            if ((*ll_i).index() == 0)
            {
                result_points.insert(std::get<0>(*ll_i));
            }
            //if one of the segments made from edges of rectangle is parallel to line l then automatically return this edge
            else if ((*ll_i).index() == 1)
            {
                return std::get<1>(*ll_i);
            }
        }
    }

    if (result_points.size() == 0)
    {
        //no intersection
        return std::nullopt;
    }
    else if (result_points.size() == 1)
    {
        //one edge had intersection
        return *result_points.begin();
    }
    else if (result_points.size() == 2)
    {
        //two edges had intersections
        std::array<point_type<T>, 2> points;
        std::copy(result_points.begin(), result_points.end(), points.begin());
        return points;
    }
    else
    {
        //rounding error made two identical points non identical
        return std::nullopt;
    }
}

template<class T>
struct points
{
    constexpr bool operator()(const point_type<T>& p1, const point_type<T>& p2) const
    {	
        return p1.x() < p2.x();
    }
};

template<class T>
std::optional<std::variant<point_type<T>, std::array<point_type<T>, 2>, segment<T>>> intersection(const rect<T>& r1, const segment<T>& s2)
{
    std::array<std::optional<std::variant<point_type<T>, segment<T>>>, 4> lr_i =
    {
        intersection(segment<T>(r1.top_left(), r1.top_right()), s2),
        intersection(segment<T>(r1.top_right(), r1.bottom_right()), s2),
        intersection(segment<T>(r1.bottom_right(), r1.bottom_left()), s2),
        intersection(segment<T>(r1.bottom_left(), r1.top_left()), s2),
    };

    std::set<point_type<T>, points<T>> result_points;
    for (auto& ll_i : lr_i)
    {
        if (ll_i)
        {
            //if there is a point of intersection between segment s and segment made from edges then add this point to the final output array
            if ((*ll_i).index() == 0)
            {
                result_points.insert(std::get<0>(*ll_i));
            }
            //if one of the segments made from edges of rectangle is parallel to segment s then automatically return segments mathematical intersection
            else if ((*ll_i).index() == 1)
            {
                return std::get<1>(*ll_i);
            }
        }
    }

    if (result_points.size() == 0)
    {
        //no intersection
        return std::nullopt;
    }
    else if (result_points.size() == 1)
    {
        //one edge had intersection
        return *result_points.begin();
    }
    else if (result_points.size() == 2)
    {
        //two edges had intersections
        std::array<point_type<T>, 2> points;
        std::copy(result_points.begin(), result_points.end(), points.begin());
        return points;
    }
    else
    {
        //rounding error made two identical points non identical
        return std::nullopt;
    }
}

template<class T>
std::optional<std::variant<rect<T>, point_type<T>, segment<T>>> intersection(const rect<T>& r1, const rect<T>& r2)
{
    auto intersection = r1.intersection(r2);
    if (!intersection)
    {
        return std::nullopt;
    }

    auto result_rect = (*intersection).normalized();

    if (std::abs(result_rect.width()) <= std::numeric_limits<T>::epsilon() && std::abs(result_rect.height()) <= std::numeric_limits<T>::epsilon())
    {
        return result_rect[0];
    }

    if (std::abs(result_rect.width()) <= std::numeric_limits<T>::epsilon() || std::abs(result_rect.height()) <= std::numeric_limits<T>::epsilon())
    {
        return segment<T>(result_rect[0], result_rect[1]);
    }

    return *intersection;
}

NAMESPACE_GEOMETRY2D_END