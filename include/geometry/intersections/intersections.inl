#pragma once

#include "intersections.hpp"
#include "../primitives/simplex.inl"
#include "../primitives/space.inl"
#include "../primitives/sphere.inl"
#include "../projections/projections.inl"

NAMESPACE_GEOMETRY_BEGIN

NAMESPACE_INTERSECTIONS_BEGIN

//space-space
template<class T, size_t D, size_t SD1, size_t SD2, size_t SD>
space_space_intersection_solution<T, D, SD1, SD2> get_subspace_intersection_result(std::vector<point_type<T, D>>&& points)
{
    if constexpr (SD == 0)
    {
        return space_space_intersection_solution<T, D, SD1, SD2>(geometry_calculation_error::linearly_dependent_vectors);
    }
    if constexpr (SD != 0)
    {
        if (points.size() == SD + 1)
        {
            space<T, SD, D> result;
            std::copy(points.begin(), points.end(), result.begin());
            return space_space_intersection_solution<T, D, SD1, SD2>(
                    space_space_intersection_solution<T, D, SD1, SD2>::subspace_intersection_result(result)
                );
        }
        else
        {
            return get_subspace_intersection_result<T, D, SD1, SD2, SD - 1>(std::forward<std::vector<point_type<T, D>>>(points));
        }
    }
}

template<class T, size_t D, size_t SD1, size_t SD2>
space_space_intersection_solution<T, D, SD1, SD2> intersection(const space<T, SD1, D>& s1, const space<T, SD2, D>& s2)
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
        for (size_t i = 0; i < SD1; i++)
        {
            equation_system[d][i] = v1s[i][d];
        }

        for (size_t i = 0; i < SD2; i++)
        {
            equation_system[d][i + SD1] = v2s[i][d];
        }
    }

    auto equation_system_solution = LINEAR_ALGEBRA::solve_equation_system(std::move(equation_system), std::move(q1p1));
    
    if (equation_system_solution)
    {
        if(equation_system_solution.system_type() == LINEAR_ALGEBRA::equation_system_type::determinate)
        {
            auto& determinate_solution = equation_system_solution.get_determinate_solution();

            if constexpr (SD1 <= SD2)
            {
                return 
                    s1[0] - 
                    std::inner_product(
                        determinate_solution.constant_solution_vector.begin(), 
                        determinate_solution.constant_solution_vector.begin() + SD1,
                        v1s.begin(), 
                        point_type<T, D>()
                    );
            }
            if constexpr (SD1 > SD2)
            {
                return 
                    s2[0] +
                    std::inner_product(
                        determinate_solution.constant_solution_vector.begin() + SD1, 
                        determinate_solution.constant_solution_vector.end(),
                        v2s.begin(),
                        point_type<T, D>()
                    );
            }
        }
        else if(equation_system_solution.system_type() == LINEAR_ALGEBRA::equation_system_type::indeterminate)
        {
            auto& indeterminate_solution = equation_system_solution.get_indeterminate_solution();

            if (indeterminate_solution.infinite_solution_vectors.size() > SD1 && indeterminate_solution.infinite_solution_vectors.size() > SD2)
            {
                //s1 or s2 has linearly dependent creating vectors
                return space_space_intersection_solution<T, D, SD1, SD2>(geometry_calculation_error::linearly_dependent_vectors);
            }

            if constexpr(SD1 <= SD2)
            {
                if (indeterminate_solution.infinite_solution_vectors.size() == SD1)
                {
                    return space_space_intersection_solution<T, D, SD1, SD2>(s1);
                }
            }

            if constexpr (SD1 > SD2)
            {
                if (indeterminate_solution.infinite_solution_vectors.size() == SD2)
                {
                    return space_space_intersection_solution<T, D, SD1, SD2>(s2);
                }
            }

            std::vector<point_type<T, D>> points(1 + indeterminate_solution.infinite_solution_vectors.size());

            if constexpr (SD1 <= SD2)
            {
                points[0] = 
                    s1[0] -
                    std::inner_product(
                        indeterminate_solution.constant_solution_vector.begin(),
                        indeterminate_solution.constant_solution_vector.begin() + SD1,
                        v1s.begin(), 
                        point_type<T, D>()
                    );

                for (size_t i = 1; i < points.size(); i++)
                {
                    points[i] = points[0] -
                        std::inner_product(
                            indeterminate_solution.infinite_solution_vectors[i - 1].begin(),
                            indeterminate_solution.infinite_solution_vectors[i - 1].begin() + SD1,
                            v1s.begin(),
                            point_type<T, D>()
                        ) +
                        std::inner_product(
                            indeterminate_solution.infinite_solution_vectors[i - 1].begin() + SD1,
                            indeterminate_solution.infinite_solution_vectors[i - 1].end(),
                            v2s.begin(),
                            point_type<T, D>()
                        );
                }

                return get_subspace_intersection_result<T, D, SD1, SD2, SD1 - 1>(std::move(points));
            }

            if constexpr (SD1 > SD2)
            {
                points[0] = 
                    s2[0] + 
                    std::inner_product(
                        indeterminate_solution.constant_solution_vector.begin() + SD1,
                        indeterminate_solution.constant_solution_vector.end(),
                        v2s.begin(), 
                        point_type<T, D>()
                    );

                for (size_t i = 1; i < points.size(); i++)
                {
                    points[i] = points[0] -
                        std::inner_product(
                            indeterminate_solution.infinite_solution_vectors[i - 1].begin(),
                            indeterminate_solution.infinite_solution_vectors[i - 1].begin() + SD1,
                            v1s.begin(), 
                            point_type<T, D>()
                        ) +
                        std::inner_product(
                            indeterminate_solution.infinite_solution_vectors[i - 1].begin() + SD1,
                            indeterminate_solution.infinite_solution_vectors[i - 1].end(),
                            v2s.begin(),
                            point_type<T, D>()
                        );
                }

                return get_subspace_intersection_result<T, D, SD1, SD2, SD2 - 1>(std::move(points));
            }
        }
    }

    return space_space_intersection_solution<T, D, SD1, SD2>();
}

//space-sphere
template<class T, size_t SD, size_t D>
space_sphere_intersection_solution<T, SD, D> intersection(const space<T, SD, D>& s1, const sphere<T, D>& s2)
{
    auto po = PROJECTIONS::project(s1, s2.center());
    if(!po)
    {
        //s1 or s2 has linearly dependent creating vectors
        return space_sphere_intersection_solution<T, SD, D>(geometry_calculation_error::linearly_dependent_vectors);
    }

    auto& p = po.get_point();

    auto d2 = (p - s2.center()).length_sqr();
    auto r2 = s2.radius() * s2.radius();

    if(d2 > r2)
    {
        //no intersection
        return space_sphere_intersection_solution<T, SD, D>();
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

        return space_sphere_intersection_solution<T, SD, D>(points);
    }
}

template<class T, size_t SD, size_t D>
space_sphere_intersection_solution<T, SD, D> intersection(const sphere<T, D>& s1, const space<T, SD, D>& s2)
{
    return intersection(s2, s1);
}

NAMESPACE_INTERSECTIONS_END

NAMESPACE_GEOMETRY_END