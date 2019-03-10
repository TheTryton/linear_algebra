#pragma once

#include "intersections.hpp"
#include "../primitives/simplex.inl"
#include "../primitives/space.inl"
#include "../primitives/sphere.inl"
#include "../projections/projections.inl"

NAMESPACE_GEOMETRY_BEGIN

NAMESPACE_INTERSECTIONS_BEGIN

//line-line
template<class T, size_t D>
std::optional<line_line_intersection<T, D>> intersection(const space<T, 1, D>& s1, const space<T, 1, D>& s2)
{
    auto p3p1 = s1.p1() - s2.p1();

    auto p2p1 = s1.p1() - s1.p2();

    auto p3p4 = s2.p2() - s2.p1();

    LINEAR_ALGEBRA::matrix<T, D, 2> equation_system;

    for (size_t d = 0; d < D; d++)
    {
        equation_system[d][0] = p2p1[d];

        equation_system[d][1] = p3p4[d];
    }

    auto result = LINEAR_ALGEBRA::solve_equation_system(std::move(equation_system), std::move(p3p1));

    if (!result)
    {
        return std::nullopt;
    }
    else
    {
        auto& res = *result;
        if (res.second.empty())
        {
            return res.first[1] * p3p4 + s2.p1();
        }
        else
        {
            if (res.second.size() == 1)
            {
                return s1;
            }
            else
            {
                //s1 or s2 has linearly dependent creating vectors
                return std::nullopt;
            }
        }
    }
}

//line-plane
template<class T, size_t D>
std::optional<line_plane_intersection<T, D>> intersection(const space<T, 1, D>& s1, const space<T, 2, D>& s2)
{
    auto p3p1 = s1.p1() - s2.p1();

    auto p2p1 = s1.p1() - s1.p2();

    auto p3p4 = s2.p2() - s2.p1();
    auto p3p5 = s2.p3() - s2.p1();

    LINEAR_ALGEBRA::matrix<T, D, 3> equation_system;

    for (size_t d = 0; d < D; d++)
    {
        equation_system[d][0] = p2p1[d];

        equation_system[d][1] = p3p4[d];
        equation_system[d][2] = p3p5[d];
    }

    auto result = LINEAR_ALGEBRA::solve_equation_system(std::move(equation_system), std::move(p3p1));

    if (!result)
    {
        return std::nullopt;
    }
    else
    {
        auto& res = *result;
        if (res.second.empty())
        {
            return s1.p1() - res.first[0]*p2p1;
        }
        else
        {
            if (res.second.size() == 1)
            {
                return s1;
            }
            else
            {
                //s1 or s2 has linearly dependent creating vectors
                return std::nullopt;
            }
        }
    }
}

template<class T, size_t D>
std::optional<line_plane_intersection<T, D>> intersection(const space<T, 2, D>& s1, const space<T, 1, D>& s2)
{
    return intersection(s2, s1);
}

//plane-plane
template<class T, size_t D>
std::optional<plane_plane_intersection<T, D>> intersection(const space<T, 2, D>& s1, const space<T, 2, D>& s2)
{
    auto p4p1 = s1.p1() - s2.p1();

    auto p2p1 = s1.p1() - s1.p2();
    auto p3p1 = s1.p1() - s1.p3();

    auto p4p5 = s2.p2() - s2.p1();
    auto p4p6 = s2.p3() - s2.p1();

    LINEAR_ALGEBRA::matrix<T, D, 4> equation_system;

    for (size_t d = 0; d < D; d++)
    {
        equation_system[d][0] = p2p1[d];
        equation_system[d][1] = p3p1[d];

        equation_system[d][2] = p4p5[d];
        equation_system[d][3] = p4p6[d];
    }

    auto result = LINEAR_ALGEBRA::solve_equation_system(std::move(equation_system), std::move(p4p1));

    if (!result)
    {
        return std::nullopt;
    }
    else
    {
        auto& res = *result;
        if (res.second.empty())
        {
            return res.first[2] * p4p5 + res.first[3] * p4p6 + s2.p1();
        }
        else
        {
            if (res.second.size() == 1)
            {
                space<T, 1, D> line;

                auto one_of_line_points = res.first[2] * p4p5 + res.first[3] * p4p6 + s2.p1();

                line.set_p1(one_of_line_points);
                line.set_p2(one_of_line_points 
                    - res.second[0][0] * p2p1 - res.second[0][1] * p3p1
                    + res.second[0][2] * p4p5 + res.second[0][3] * p4p6
                );

                return line;
            }
            else if (res.second.size() == 2)
            {
                return s1;
            }
            else
            {
                //s1 or s2 has linearly dependent creating vectors
                return std::nullopt;
            }
        }
    }
}

//space-space
template<class T, size_t D, size_t SD1, size_t SD2, size_t SD>
space_space_intersection<T, D, SD1, SD2> get_result_intersection(std::vector<point_type<T, D>>&& points)
{
    if constexpr (SD == 0)
    {
        return point_type<T, D>();
    }
    if constexpr (SD != 0)
    {
        if (points.size() == SD + 1)
        {
            space<T, SD, D> result;
            std::copy(points.begin(), points.end(), result.begin());
            return result;
        }
        else
        {
            return get_result_intersection<T, D, SD1, SD2, SD - 1>(std::forward<std::vector<point_type<T, D>>>(points));
        }
    }
}

template<class T, size_t D, size_t SD1, size_t SD2>
std::optional<space_space_intersection<T, D, SD1, SD2>> intersection(const space<T, SD1, D>& s1, const space<T, SD2, D>& s2)
{
    auto q1p1 = s1[0] - s2[0];

    std::array<vector_type<T, D>, SD1> v1s;
    std::array<vector_type<T, D>, SD2> v2s;

    for (size_t i = 1; i <= SD1; i++)
    {
        v1s[i - 1] = s1[0] - s1[i];
    }

    for (size_t i = 1; i <= SD2; i++)
    {
        v2s[i - 1] = s2[i] - s2[0];
    }

    LINEAR_ALGEBRA::matrix<T, D, SD1 + SD2> equation_system;

    for (size_t d = 0; d < D; d++)
    {
        for (size_t i = 0; i < v1s.size(); i++)
        {
            equation_system[d][i] = v1s[i][d];
        }

        for (size_t i = 0; i < v2s.size(); i++)
        {
            equation_system[d][i + SD1] = v2s[i][d];
        }
    }

    auto result = LINEAR_ALGEBRA::solve_equation_system(std::move(equation_system), std::move(q1p1));
    
    if (!result)
    {
        return std::nullopt;
    }
    else
    {
        auto& res = *result;
        if (res.second.empty())
        {
            if constexpr (SD1 <= SD2)
            {
                return s1[0] - std::inner_product(res.first.begin(), res.first.begin() + SD1, v1s.begin(), point_type<T, D>());
            }
            if constexpr (SD1 > SD2)
            {
                return s2[0] + std::inner_product(res.first.begin() + SD1, res.first.end(), v2s.begin(), point_type<T, D>());
            }
        }
        else
        {
            if constexpr(SD1 <= SD2)
            {
                if (res.second.size() == SD1)
                {
                    return s1;
                }
            }

            if constexpr (SD1 > SD2)
            {
                if (res.second.size() == SD2)
                {
                    return s2;
                }
            }
            
            if(res.second.size() > SD1 && res.second.size() > SD2)
            {
                //s1 or s2 has linearly dependent creating vectors
                return std::nullopt;
            }
            else
            {
                std::vector<point_type<T, D>> points(1 + res.second.size());

                if constexpr (SD1 <= SD2)
                {
                    points[0] = s1[0] - std::inner_product(res.first.begin(), res.first.begin() + SD1, v1s.begin(), point_type<T, D>());

                    for (size_t i = 1; i < points.size(); i++)
                    {
                        points[i] = points[0] -
                            std::inner_product(res.second[i - 1].begin(), res.second[i - 1].begin() + SD1, v1s.begin(), point_type<T, D>()) +
                            std::inner_product(res.second[i - 1].begin() + SD1, res.second[i - 1].end(), v2s.begin(), point_type<T, D>());
                    }

                    return get_result_intersection<T, D, SD1, SD2, SD1>(std::move(points));
                }
                if constexpr (SD1 > SD2)
                {
                    points[0] = s2[0] + std::inner_product(res.first.begin() + SD1, res.first.end(), v2s.begin(), point_type<T, D>());

                    for (size_t i = 1; i < points.size(); i++)
                    {
                        points[i] = points[0] -
                            std::inner_product(res.second[i - 1].begin(), res.second[i - 1].begin() + SD1, v1s.begin(), point_type<T, D>()) +
                            std::inner_product(res.second[i - 1].begin() + SD1, res.second[i - 1].end(), v2s.begin(), point_type<T, D>());
                    }

                    return get_result_intersection<T, D, SD1, SD2, SD2>(std::move(points));
                }
            }
        }
    }
}

//space-sphere
template<class T, size_t SD, size_t D>
std::optional<space_sphere_intersection<T, SD, D>> intersection(const space<T, SD, D>& s1, const sphere<T, D>& s2)
{
    auto po = PROJECTIONS::project(s1, s2.center());
    if(!po)
    {
        return std::nullopt;
    }

    auto& p = *po;

    auto d2 = (p - s2.center()).length_sqr();
    auto r2 = s2.radius() * s2.radius();

    if(d2 > r2)
    {
        return std::nullopt;
    }
    else if (LINEAR_ALGEBRA::equal(d2, r2))
    {
        return p;
    }
    else
    {
        auto l = LINEAR_ALGEBRA::functions_implementation<T>::sqrt(r2 - d2);

        std::array<point_type<T, D>, SD + 1> points;

        std::transform(s1.begin() + 1, s1.end(), points.begin(), [&p, &l, &s1](const point_type<T, D>& pt) {
            auto v = pt - s1[0];
            v.normalize();
            v *= l;
            return p + v;
        });

        points.back() = static_cast<T>(2) * p - points[0];

        return points;
    }
}

template<class T, size_t SD, size_t D>
std::optional<space_sphere_intersection<T, SD, D>> intersection(const sphere<T, D>& s1, const space<T, SD, D>& s2)
{
    return intersection(s2, s1);
}

NAMESPACE_INTERSECTIONS_END

NAMESPACE_GEOMETRY_END