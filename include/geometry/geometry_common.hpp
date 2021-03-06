#pragma once

#include <system_error>

#include "../linear_algebra/linear_algebra.hpp"

#define GEOMETRY geometry
#define NAMESPACE_GEOMETRY_BEGIN namespace GEOMETRY{
#define NAMESPACE_GEOMETRY_END }

#define GEOMETRY_IO geometry_io
#define NAMESPACE_GEOMETRY_IO_BEGIN namespace GEOMETRY_IO{
#define NAMESPACE_GEOMETRY_IO_END }

#define PROJECTIONS projections
#define NAMESPACE_PROJECTIONS_BEGIN namespace PROJECTIONS{
#define NAMESPACE_PROJECTIONS_END }

#define INTERSECTIONS intersections
#define NAMESPACE_INTERSECTIONS_BEGIN namespace INTERSECTIONS{
#define NAMESPACE_INTERSECTIONS_END }

NAMESPACE_GEOMETRY_BEGIN

template<class T, size_t D>
using point_type = LINEAR_ALGEBRA::vector<T, D>;
template<class T, size_t D>
using vector_type = LINEAR_ALGEBRA::vector<T, D>;

enum inclusion_mode
{
    edgeless,
    edge_only,
    full
};

///<summary>
/// {SD}-dimensional space in {D}-dimensional vector space (T as underlying mathematical field)
/// <para>*** TEMPLATE PARAMETERS ***</para>
/// <para>T - underlying arithmetic type (underlying mathematical field type for vectors and points)</para>
/// <para>SD - space dimensions (i.e 0 -> point, 1 -> line, 2 -> plane, etc.) </para>
/// <para>D - dimension of vector space in which plane is given </para>
/// <para>*** REMARKS ***</para>
/// <para>SD must be less or equal to D</para>
///</summary>
template<class T, size_t PD, size_t D>
class space;

template<class T, size_t D>
using line = space<T, 1, D>;

template<class T, size_t D>
using plane = space<T, 2, D>;

///<summary>
/// {SD}-dimensional simplex in {D}-dimensional vector space (T as underlying mathematical field)
/// <para>*** TEMPLATE PARAMETERS ***</para>
/// <para>T - underlying arithmetic type (underlying mathematical field type for vectors and points)</para>
/// <para>SD - simplex dimensions (i.e 0 -> point, 1 -> segment, 2 -> triangle, etc.) </para>
/// <para>D - dimension of vector space in which simplex is given </para>
/// <para>*** REMARKS ***</para>
/// <para>SD must be less or equal to D</para>
///</summary>
template<class T, size_t SD, size_t D>
class simplex;

template<class T, size_t D>
using segment = simplex<T, 1, D>;

template<class T, size_t D>
using triangle = simplex<T, 2, D>;

template<class T, size_t D>
using tetrahedron = simplex<T, 3, D>;

///<summary>
/// {D}-dimensional sphere in {D}-dimensional vector space (T as underlying mathematical field)
/// <para>*** TEMPLATE PARAMETERS ***</para>
/// <para>T - underlying arithmetic type (underlying mathematical field type for vectors and points)</para>
/// <para>D - dimension of sphere and vector space in which sphere is given </para>
///</summary>
template<class T, size_t D>
class sphere;

enum class geometry_calculation_error
{
    none,
    linearly_dependent_vectors,
};

struct geometry_calculation_error_category : std::error_category
{
    virtual const char* name() const noexcept override;
    virtual std::string message(int e) const override;
};

const geometry_calculation_error_category geometry_calculation_error_category_instance{};

class geometry_calculation
{
protected:
    std::error_code _code;
public:
    geometry_calculation();
    geometry_calculation(geometry_calculation_error error);
public:
    std::error_code get_error_code() const;
};

NAMESPACE_GEOMETRY_END

namespace std
{
    template<>
    struct is_error_code_enum<GEOMETRY::geometry_calculation_error> : true_type {};
}

std::error_code make_error_code(GEOMETRY::geometry_calculation_error error);