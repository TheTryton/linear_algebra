#pragma once

#include "../geometry_common.hpp"

NAMESPACE_GEOMETRY_BEGIN

NAMESPACE_PROJECTIONS_BEGIN

template<class T, size_t D>
class projection_solution : public geometry_calculation
{
public:
    using point_projection_result = point_type<T, D>;
private:
    using solution_type = std::optional<point_projection_result>;
    solution_type _solution;
public:
    projection_solution() :
        geometry_calculation()
    {
    }
    projection_solution(geometry_calculation_error error) :
        geometry_calculation(error)
    {
    }

    projection_solution(const projection_solution& other) :
        _solution(other._solution),
        geometry_calculation(other)
    {
    }
    projection_solution(projection_solution&& other) :
        _solution(std::move(other._solution)),
        geometry_calculation(other)
    {
    }
    projection_solution& operator=(const projection_solution& other)
    {
        _solution = other._solution;
        _code = other._code;
        return *this;
    }
    projection_solution& operator=(projection_solution&& other)
    {
        _solution = std::move(other._solution);
        _code = std::move(other._code);
        return *this;
    }

    projection_solution(const point_projection_result& point_projection_result) :
        _solution(point_projection_result),
        geometry_calculation()
    {
    }
    projection_solution(point_projection_result&& point_projection_result) :
        _solution(point_projection_result),
        geometry_calculation()
    {
    }
    projection_solution& operator=(const point_projection_result& point_projection_result)
    {
        _solution = point_projection_result;
        return *this;
    }
    projection_solution& operator=(point_projection_result&& point_projection_result)
    {
        _solution = point_projection_result;
        return *this;
    }
public:
    point_projection_result& get_point()
    {
        return *_solution;
    }
    const point_projection_result& get_point() const
    {
        return *_solution;
    }
public:
    operator bool() const
    {
        return static_cast<bool>(_solution) && (static_cast<geometry_calculation_error>(get_error_code().value()) != geometry_calculation_error::linearly_dependent_vectors);
    }
};

//point on space
template<class T, size_t SD, size_t D>
projection_solution<T, D> project(const space<T, SD, D>& s, const point_type<T, D>& p);

template<class T, size_t SD, size_t D>
projection_solution<T, D> project(const point_type<T, D>& p, const space<T, SD, D>& s);

//point on simplex
template<class T, size_t SD, size_t D>
projection_solution<T, D> closest_point(const simplex<T, SD, D>& s, const point_type<T, D>& p);

template<class T, size_t SD, size_t D>
projection_solution<T, D> closest_point(const point_type<T, D>& p, const simplex<T, SD, D>& s);

NAMESPACE_PROJECTIONS_END

NAMESPACE_GEOMETRY_END