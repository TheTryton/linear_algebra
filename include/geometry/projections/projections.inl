#pragma once

#include "projections.hpp"
#include "../primitives/simplex.inl"
#include "../primitives/space.inl"

NAMESPACE_GEOMETRY_BEGIN

NAMESPACE_PROJECTIONS_BEGIN

template<class T, size_t SD, size_t D>
projection_solution<T, D> project(const space<T, SD, D>& s, const point_type<T, D>& p)
{
    std::array<vector_type<T, D>, SD> simplex_vectors;
    
    std::transform(s.begin() + 1, s.end(), simplex_vectors.begin(), [&s](const point_type<T, D>& sp) {
        return sp - s[0];
    });

    LINEAR_ALGEBRA::matrix<T, SD, SD> equation_system;

    for (size_t diagonal = 0; diagonal < SD; diagonal++)
    {
        equation_system[diagonal][diagonal] = simplex_vectors[diagonal] * simplex_vectors[diagonal];
    }

    for (size_t row = 0; row < SD; row++)
    {
        for (size_t column = row + 1; column < SD; column++)
        {
            auto inner_product = simplex_vectors[column] * simplex_vectors[row];
            equation_system[row][column] = inner_product;
            equation_system[column][row] = inner_product;
        }
    }

    vector_type<T, SD> constant_terms;

    auto v1p = p - s[0];

    std::transform(simplex_vectors.begin(), simplex_vectors.end(), constant_terms.begin(), [&v1p](const vector_type<T, D>& v) {
        return v1p * v;
    });

    auto inverse = equation_system.inverted();
    if(!inverse)
    {
        return projection_solution<T, D>(geometry_calculation_error::linearly_dependent_vectors);
    }
    else
    {
        auto equation_system_solution = *inverse * constant_terms;

        return projection_solution<T, D>(
            projection_solution<T, D>::point_projection_result(
                std::inner_product(
                    simplex_vectors.begin(),
                    simplex_vectors.end(),
                    equation_system_solution.begin(),
                    s[0]
                )
                )
            );
    }
}

template<class T, size_t SD, size_t D>
projection_solution<T, D> project(const point_type<T, D>& p, const space<T, SD, D>& s)
{
    return project(s, p);
}

template<class T, size_t SD, size_t D>
projection_solution<T, D> closest_point(const simplex<T, SD, D>& s, const point_type<T, D>& p)
{
    if constexpr(SD == 1)
    {
        auto projection = project(static_cast<space<T, SD, D>>(s), p);

        if (projection)
        {
            if (s.contains(projection.get_point()))
            {
                return projection;
            }
            else
            {
                return *std::min_element(s.begin(), s.end(), [&p](const point_type<T, D>& pt1, const point_type<T, D>& pt2) {
                    return (pt1 - p).length_sqr() < (pt2 - p).length_sqr();
                });
            }
        }
    }

    if constexpr(SD != 1)
    {
        auto projection = project(static_cast<space<T, SD, D>>(s), p);

        if (projection)
        {
            if (s.contains(projection.get_point()))
            {
                return projection;
            }
            else
            {
                std::array<simplex<T, SD - 1, D>, SD + 1> sub_simplices;
                for (size_t sp = 0; sp < SD + 1; sp++)
                {
                    for (size_t v = 0; v < SD; v++)
                    {
                        sub_simplices[sp][v] = s[(v + sp) % (SD + 1)];
                    }
                }

                std::array<std::optional<point_type<T, D>>, SD + 1> projections;
                for (size_t i = 0; i < sub_simplices.size(); i++)
                {
                    projections[i] = closest_point(sub_simplices[i], projection.get_point());
                }

                return **std::min_element(projections.begin(), projections.end(), [&p](
                    const std::optional<point_type<T, D>>& pt1,
                    const std::optional<point_type<T, D>>& pt2
                    ) 
                {
                    if(!pt1)
                    {
                        return false;
                    }
                    if(!pt2)
                    {
                        return true;
                    }

                    return (*pt1 - p).length_sqr() < (*pt2 - p).length_sqr();
                });
            }
        }
    }

    return projection_solution<T, D>(geometry_calculation_error::linearly_dependent_vectors);
}

template<class T, size_t SD, size_t D>
projection_solution<T, D> closest_point(const point_type<T, D>& p, const simplex<T, SD, D>& s)
{
    return closest_point(s, p);
}

NAMESPACE_PROJECTIONS_END

NAMESPACE_GEOMETRY_END