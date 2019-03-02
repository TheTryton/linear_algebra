#pragma once

#include <array>
#include <variant>
#include <set>

#include "../linear_algebra/linear_algebra.hpp"

#define GEOMETRY2D geometry2D
#define NAMESPACE_GEOMETRY2D_BEGIN namespace GEOMETRY2D{
#define NAMESPACE_GEOMETRY2D_END }

NAMESPACE_GEOMETRY2D_BEGIN

template<class T>
using point_type = LINEAR_ALGEBRA::vector<T, 2>;
template<class T>
using vector_type = LINEAR_ALGEBRA::vector<T, 2>;

enum inclusion_mode
{
    edgeless,
    edge_only,
    full
};

template<class T>
class line;

template<class T>
class segment;

template<class T>
class triangle;

template<class T>
class rect;

NAMESPACE_GEOMETRY2D_END