#pragma once

#include "generalized_vector_product.hpp"
#include "../primitives/simplex.inl"
#include "../primitives/space.inl"

NAMESPACE_GEOMETRY_BEGIN

template<class T, size_t D, size_t K, size_t KC>
perpendicular_solution<T, D, K> get_result_perpendicular(std::vector<vector_type<T, D>>&& vectors)
{
    if constexpr (KC == D - K - 1)
    {
        return perpendicular_solution<T, D, K>(geometry_calculation_error::linearly_dependent_vectors);
    }
    if constexpr (KC != D - K - 1)
    {
        if (vectors.size() == KC)
        {
            space<T, KC, D> result;
            std::copy(vectors.begin(), vectors.end(), result.begin() + 1);
            return perpendicular_solution<T, D, K>(
                    perpendicular_solution<T, D, K>::perpendicular_result(result)
                );
        }
        else
        {
            return get_result_perpendicular<T, D, K, KC - 1>(std::forward<std::vector<vector_type<T, D>>>(vectors));
        }
    }
}

template<class T, size_t D, class... TOS, typename>
perpendicular_solution<T, D, sizeof...(TOS) + 1> perpendicular(const vector_type<T, D>& v, const vector_type<TOS, D>&... vs)
{
    constexpr size_t K = sizeof...(TOS) + 1;

    std::array<vector_type<T, D>, K> vectors = { v, static_cast<vector_type<T, D>>(vs)... };

    LINEAR_ALGEBRA::matrix<T, K, D> set_of_vectors;

    for (size_t v = 0; v < K; v++)
    {
        for (size_t d = 0; d < D; d++)
        {
            set_of_vectors[v][d] = vectors[v][d];
        }
    }

    vector_type<T, K> constant_terms;

    auto equation_system_solution = LINEAR_ALGEBRA::solve_equation_system(std::move(set_of_vectors), std::move(constant_terms));

    if (equation_system_solution)
    {
        if (equation_system_solution.system_type() == LINEAR_ALGEBRA::equation_system_type::indeterminate)
        {
            return perpendicular_solution<T, D, sizeof...(TOS) + 1>(
                    get_result_perpendicular<T, D, K, D - 1>(std::move(equation_system_solution.get_indeterminate_solution().infinite_solution_vectors))
                );
        }
    }

    return perpendicular_solution<T, D, sizeof...(TOS) + 1>(geometry_calculation_error::linearly_dependent_vectors);
}

template<class T, size_t D, size_t K>
perpendicular_solution<T, D, K> perpendicular(const std::array<vector_type<T, D>, K>& vectors)
{
    LINEAR_ALGEBRA::matrix<T, K, D> set_of_vectors;

    for (size_t v = 0; v < K; v++)
    {
        for (size_t d = 0; d < D; d++)
        {
            set_of_vectors[v][d] = vectors[v][d];
        }
    }

    vector_type<T, K> constant_terms;

    auto result = LINEAR_ALGEBRA::solve_equation_system(std::move(set_of_vectors), std::move(constant_terms));

    if (!result)
    {
        return std::nullopt;
    }
    else
    {
        auto r = *result;

        if (!r.second.empty())
        {
            return get_result_perpendicular<T, D, K, D - 1>(std::move(r.second));
        }
        else
        {
            return std::nullopt;
        }
    }
}

NAMESPACE_GEOMETRY_END