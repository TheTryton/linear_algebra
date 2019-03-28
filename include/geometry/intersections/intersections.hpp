#pragma once

#include "../geometry_common.hpp"

NAMESPACE_GEOMETRY_BEGIN

NAMESPACE_INTERSECTIONS_BEGIN

enum class space_space_intersection_type
{
    no_intersection, // two spaces do not intersect
    point, // two spaces intersect in point
    subspace, // two spaces intersect creating space of lower dimension
    space // two space intersect in such way that on of the spaces is contained within other
};

template<class T, size_t D, size_t SD1, size_t SD2>
class space_space_intersection_solution : public geometry_calculation
{
private:
    template<class T, size_t D, class SD>
    struct subspace_intersection_result_impl
    {
        using subspace_intersection_result_type = point_type<T, D>;
    };

    template<class T, size_t D>
    struct subspace_intersection_result_impl<T, D, std::index_sequence<>>
    {
        using subspace_intersection_result_type = std::variant<char>;
    };

    template<class T, size_t D, size_t... SD>
    struct subspace_intersection_result_impl<T, D, std::index_sequence<SD...>>
    {
        using subspace_intersection_result_type = std::variant<space<T, SD + 1, D>...>;
    };
public:
    static constexpr size_t smaller_space_dimension = SD1 <= SD2 ? SD1 : SD2;
    static constexpr size_t larger_space_dimension = SD1 >= SD2 ? SD1 : SD2;
public:
    using point_intersection_result = point_type<T, D>;
    using subspace_intersection_result = typename subspace_intersection_result_impl < T, D,
        decltype(std::make_index_sequence <
            smaller_space_dimension - 1
        >())
    >::subspace_intersection_result_type;
    using space_intersection_result = space<T, smaller_space_dimension, D>;
private:
    space_space_intersection_type _type;
    using solution_type = std::optional<std::variant<point_intersection_result, subspace_intersection_result, space_intersection_result>>;
    solution_type _solution;
public:
    space_space_intersection_solution():
        _type(space_space_intersection_type::no_intersection),
        geometry_calculation()
    {
    }
    space_space_intersection_solution(geometry_calculation_error error) :
        _type(space_space_intersection_type::no_intersection),
        geometry_calculation(error)
    {
    }

    space_space_intersection_solution(const space_space_intersection_solution& other) :
        _type(other._type),
        _solution(other._solution),
        geometry_calculation(other)
    {
    }
    space_space_intersection_solution(space_space_intersection_solution&& other) :
        _type(std::move(other._type)),
        _solution(std::move(other._solution)),
        geometry_calculation(other)
    {
        other._type = space_space_intersection_type::no_intersection;
    }
    space_space_intersection_solution& operator=(const space_space_intersection_solution& other)
    {
        _type = other._type;
        _solution = other._solution;
        _code = other._code;
        return *this;
    }
    space_space_intersection_solution& operator=(space_space_intersection_solution&& other)
    {
        _type = std::move(other._type);
        _solution = std::move(other._solution);
        _code = std::move(other._code);
        other._type = space_space_intersection_type::no_intersection;
        return *this;
    }

    space_space_intersection_solution(const point_intersection_result& point_intersection_result) :
        _type(space_space_intersection_type::point),
        _solution(point_intersection_result),
        geometry_calculation()
    {
    }
    space_space_intersection_solution(point_intersection_result&& point_intersection_result) :
        _type(space_space_intersection_type::point),
        _solution(point_intersection_result),
        geometry_calculation()
    {
    }
    space_space_intersection_solution& operator=(const point_intersection_result& point_intersection_result)
    {
        _type = space_space_intersection_type::point;
        _solution = point_intersection_result;
        return *this;
    }
    space_space_intersection_solution& operator=(point_intersection_result&& point_intersection_result)
    {
        _type = space_space_intersection_type::point;
        _solution = point_intersection_result;
        return *this;
    }

    space_space_intersection_solution(const subspace_intersection_result& subspace_intersection_result) :
        _type(space_space_intersection_type::subspace),
        _solution(subspace_intersection_result),
        geometry_calculation()
    {
    }
    space_space_intersection_solution(subspace_intersection_result&& subspace_intersection_result) :
        _type(space_space_intersection_type::subspace),
        _solution(subspace_intersection_result),
        geometry_calculation()
    {
    }
    space_space_intersection_solution& operator=(const subspace_intersection_result& subspace_intersection_result)
    {
        _type = space_space_intersection_type::subspace;
        _solution = subspace_intersection_result;
        return *this;
    }
    space_space_intersection_solution& operator=(subspace_intersection_result&& subspace_intersection_result)
    {
        _type = space_space_intersection_type::subspace;
        _solution = subspace_intersection_result;
        return *this;
    }

    space_space_intersection_solution(const space_intersection_result& space_intersection_result) :
        _type(space_space_intersection_type::space),
        _solution(space_intersection_result),
        geometry_calculation()
    {
    }
    space_space_intersection_solution(space_intersection_result&& space_intersection_result) :
        _type(space_space_intersection_type::space),
        _solution(space_intersection_result),
        geometry_calculation()
    {
    }
    space_space_intersection_solution& operator=(const space_intersection_result& space_intersection_result)
    {
        _type = space_space_intersection_type::space;
        _solution = space_intersection_result;
        return *this;
    }
    space_space_intersection_solution& operator=(space_intersection_result&& space_intersection_result)
    {
        _type = space_space_intersection_type::space;
        _solution = space_intersection_result;
        return *this;
    }
public:
    space_space_intersection_type intersection_type() const
    {
        return _type;
    }

    point_intersection_result& get_point()
    {
        return std::get<0>(*_solution);
    }
    const point_intersection_result& get_point() const
    {
        return std::get<0>(*_solution);
    }

    size_t get_subspace_dimension() const
    {
        return std::get<1>(*_solution).index() + 1;
    }

    template<size_t SSD, typename = typename std::enable_if_t<(SSD > 0 && SSD < smaller_space_dimension)>>
    space<T, SSD, D>& get_subspace()
    {
        return std::get<SSD - 1>(std::get<1>(*_solution));
    }
    template<size_t SSD, typename = typename std::enable_if_t<(SSD > 0 && SSD < smaller_space_dimension)>>
    const space<T, SSD, D>& get_subspace() const
    {
        return std::get<SSD - 1>(std::get<1>(*_solution));
    }

    space<T, smaller_space_dimension, D>& get_space()
    {
        return std::get<2>(*_solution);
    }
    const space<T, smaller_space_dimension, D>& get_space() const
    {
        return std::get<2>(*_solution);
    }
public:
    operator bool() const
    {
        return static_cast<bool>(_solution) && (_type != space_space_intersection_type::no_intersection);
    }
};

enum class space_sphere_intersection_type
{
    no_intersection, // space and sphere do not intersect
    point, // space is tangent to sphere
    sphere_creating_points, // space intersect sphere creating SD + 1 points that create lower dimension sphere
};

template<class T, size_t SD, size_t D>
class space_sphere_intersection_solution : public geometry_calculation
{
public:
    using point_intersection_result = point_type<T, D>;
    using sphere_creating_points_intersection_result = std::array<point_type<T, D>, SD + 1>;
private:
    space_sphere_intersection_type _type;
    using solution_type = std::optional<std::variant<point_intersection_result, sphere_creating_points_intersection_result>>;
    solution_type _solution;
public:
    space_sphere_intersection_solution() :
        _type(space_sphere_intersection_type::no_intersection),
        geometry_calculation()
    {
    }
    space_sphere_intersection_solution(geometry_calculation_error error) :
        _type(space_sphere_intersection_type::no_intersection),
        geometry_calculation(error)
    {
    }

    space_sphere_intersection_solution(const space_sphere_intersection_solution& other) :
        _type(other._type),
        _solution(other._solution),
        geometry_calculation(other)
    {
    }
    space_sphere_intersection_solution(space_sphere_intersection_solution&& other) :
        _type(std::move(other._type)),
        _solution(std::move(other._solution)),
        geometry_calculation(other)
    {
        other._type = space_sphere_intersection_type::no_intersection;
    }
    space_sphere_intersection_solution& operator=(const space_sphere_intersection_solution& other)
    {
        _type = other._type;
        _solution = other._solution;
        _code = other._code;
        return *this;
    }
    space_sphere_intersection_solution& operator=(space_sphere_intersection_solution&& other)
    {
        _type = std::move(other._type);
        _solution = std::move(other._solution);
        _code = std::move(other._code);
        other._type = space_sphere_intersection_type::no_intersection;
        return *this;
    }

    space_sphere_intersection_solution(const point_intersection_result& point_intersection_result) :
        _type(space_sphere_intersection_type::point),
        _solution(point_intersection_result),
        geometry_calculation()
    {
    }
    space_sphere_intersection_solution(point_intersection_result&& point_intersection_result) :
        _type(space_sphere_intersection_type::point),
        _solution(point_intersection_result),
        geometry_calculation()
    {
    }
    space_sphere_intersection_solution& operator=(const point_intersection_result& point_intersection_result)
    {
        _type = space_sphere_intersection_type::point;
        _solution = point_intersection_result;
        return *this;
    }
    space_sphere_intersection_solution& operator=(point_intersection_result&& point_intersection_result)
    {
        _type = space_sphere_intersection_type::point;
        _solution = point_intersection_result;
        return *this;
    }

    space_sphere_intersection_solution(const sphere_creating_points_intersection_result& sphere_creating_points_intersection_result) :
        _type(space_sphere_intersection_type::sphere_creating_points),
        _solution(sphere_creating_points_intersection_result),
        geometry_calculation()
    {
    }
    space_sphere_intersection_solution(sphere_creating_points_intersection_result&& sphere_creating_points_intersection_result) :
        _type(space_sphere_intersection_type::sphere_creating_points),
        _solution(sphere_creating_points_intersection_result),
        geometry_calculation()
    {
    }
    space_sphere_intersection_solution& operator=(const sphere_creating_points_intersection_result& sphere_creating_points_intersection_result)
    {
        _type = space_sphere_intersection_type::sphere_creating_points;
        _solution = sphere_creating_points_intersection_result;
        return *this;
    }
    space_sphere_intersection_solution& operator=(sphere_creating_points_intersection_result&& sphere_creating_points_intersection_result)
    {
        _type = space_sphere_intersection_type::sphere_creating_points;
        _solution = sphere_creating_points_intersection_result;
        return *this;
    }
public:
    space_sphere_intersection_type intersection_type() const
    {
        return _type;
    }
    
    point_intersection_result& get_point()
    {
        return std::get<0>(*_solution);
    }
    const point_intersection_result& get_point() const
    {
        return std::get<0>(*_solution);
    }

    sphere_creating_points_intersection_result& get_sphere_creating_points()
    {
        return std::get<1>(*_solution);
    }
    const sphere_creating_points_intersection_result& get_sphere_creating_points() const
    {
        return std::get<1>(*_solution);
    }
public:
    operator bool() const
    {
        return static_cast<bool>(_solution) && (_type != space_sphere_intersection_type::no_intersection);
    }
};

//space-space
template<class T, size_t D, size_t SD1, size_t SD2>
space_space_intersection_solution<T, D, SD1, SD2> intersection(const space<T, SD1, D>& s1, const space<T, SD2, D>& s2);

//space-sphere
template<class T, size_t SD, size_t D>
space_sphere_intersection_solution<T, SD, D> intersection(const space<T, SD, D>& s1, const sphere<T, D>& s2);

template<class T, size_t SD, size_t D>
space_sphere_intersection_solution<T, SD, D> intersection(const sphere<T, D>& s1, const space<T, SD, D>& s2);

NAMESPACE_INTERSECTIONS_END

NAMESPACE_GEOMETRY_END