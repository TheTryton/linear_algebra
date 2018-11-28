#pragma once

#include "vector/vector.hpp"
#include "matrix/matrix.hpp"

#include "common_matrix_vector_operations.inl"

using matrix2f = matrix<float, 2, 2>;
using matrix3f = matrix<float, 3, 3>;
using matrix4f = matrix<float, 4, 4>;

using matrix2d = matrix<double, 2, 2>;
using matrix3d = matrix<double, 3, 3>;
using matrix4d = matrix<double, 4, 4>;

using vector2f = vector<float, 2>;
using vector3f = vector<float, 3>;
using vector4f = vector<float, 4>;