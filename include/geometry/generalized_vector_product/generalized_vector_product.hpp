#pragma once

#include "../geometry_common.hpp"
#include "../primitives/space.inl"

NAMESPACE_GEOMETRY_BEGIN

template<class T, size_t D, size_t K>
class perpendicular_solution : public geometry_calculation
{
private:
    template<class T, size_t D, class K>
    struct perpendicular_result_impl
    {
        using perpendicular_result_type = void;
    };

    template<class T, size_t D, size_t... K>
    struct perpendicular_result_impl<T, D, std::index_sequence<K...>>
    {
        using perpendicular_result_type = std::variant<space<T, D - K - 1, D>...>;
    };
public:
    using perpendicular_result = typename perpendicular_result_impl<
            T,
            D,
            decltype(std::make_index_sequence<K>())
        >::perpendicular_result_type;
private:
    using solution_type = std::optional<perpendicular_result>;
    solution_type _solution;
public:
    perpendicular_solution() :
        geometry_calculation()
    {
    }
    perpendicular_solution(geometry_calculation_error error) :
        geometry_calculation(error)
    {
    }

    perpendicular_solution(const perpendicular_solution& other) :
        _solution(other._solution),
        geometry_calculation(other)
    {
    }
    perpendicular_solution(perpendicular_solution&& other) :
        _solution(std::move(other._solution)),
        geometry_calculation(other)
    {
    }
    perpendicular_solution& operator=(const perpendicular_solution& other)
    {
        _solution = other._solution;
        _code = other._code;
        return *this;
    }
    perpendicular_solution& operator=(perpendicular_solution&& other)
    {
        _solution = std::move(other._solution);
        _code = std::move(other._code);
        return *this;
    }

    perpendicular_solution(const perpendicular_result& perpendicular_result) :
        _solution(perpendicular_result),
        geometry_calculation()
    {
    }
    perpendicular_solution(perpendicular_result&& perpendicular_result) :
        _solution(perpendicular_result),
        geometry_calculation()
    {
    }
    perpendicular_solution& operator=(const perpendicular_result& perpendicular_result)
    {
        _solution = perpendicular_result;
        return *this;
    }
    perpendicular_solution& operator=(perpendicular_result&& perpendicular_result)
    {
        _solution = perpendicular_result;
        return *this;
    }
public:
    size_t get_perpendicular_subspace_dimension() const
    {
        return D + 1 - K - _solution->index();
    }

    template<size_t SSD, typename = typename std::enable_if_t<(SSD > 0 && SSD < D)>>
    space<T, SSD, D>& get_subspace()
    {
        return std::get<D + 1 - K - SSD>(*_solution);
    }
    template<size_t SSD, typename = typename std::enable_if_t<(SSD > 0 && SSD < D)>>
    const space<T, SSD, D>& get_subspace() const
    {
        return std::get<D + 1 - K - SSD>(*_solution);
    }
public:
    operator bool() const
    {
        return static_cast<bool>(_solution) && (static_cast<geometry_calculation_error>(get_error_code().value()) != geometry_calculation_error::linearly_dependent_vectors);
    }
};

template<class T, size_t D, class... TOS, typename = typename std::enable_if_t<(sizeof...(TOS) + 1 < D)>>
perpendicular_solution<T, D, sizeof...(TOS) + 1> perpendicular(const vector_type<T, D>& v, const vector_type<TOS, D>&... vs);

template<class T, size_t D, size_t K>
perpendicular_solution<T, D, K> perpendicular(const std::array<vector_type<T, D>, K>& vectors);

NAMESPACE_GEOMETRY_END