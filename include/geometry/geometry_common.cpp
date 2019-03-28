#include "geometry_common.hpp"

NAMESPACE_GEOMETRY_BEGIN

const char* geometry_calculation_error_category::name() const noexcept
{
    return "geometry calculation error";
}

std::string geometry_calculation_error_category::message(int e) const
{
    switch (static_cast<geometry_calculation_error>(e))
    {
    case geometry_calculation_error::none:
        return "none";
    case geometry_calculation_error::linearly_dependent_vectors:
        return "linearly dependent vectors";
    }
    return "unrecognized error";
}

geometry_calculation::geometry_calculation() :
    _code(make_error_code(geometry_calculation_error::none))
{
}

geometry_calculation::geometry_calculation(geometry_calculation_error error) :
    _code(make_error_code(error))
{
}

std::error_code geometry_calculation::get_error_code() const
{
    return _code;
}

NAMESPACE_GEOMETRY_END

std::error_code make_error_code(GEOMETRY::geometry_calculation_error error)
{
    return std::error_code(static_cast<int>(error), GEOMETRY::geometry_calculation_error_category_instance);
}
