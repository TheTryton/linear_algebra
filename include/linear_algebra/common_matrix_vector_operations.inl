#pragma once

#include "vector/vector.hpp"
#include "matrix/matrix.hpp"

NAMESPACE_LINEAR_ALGEBRA_BEGIN

template<class T, size_t N, size_t M>
template<class TO>
auto matrix<T, N, M>::operator*(const vector<TO, M>& vec) const
{
    using cm_t = decltype(std::declval<T>()*std::declval<TO>() + std::declval<T>()*std::declval<TO>());
    vector<cm_t, N> result;
    for (size_t row = 0; row < N; row++)
    {
        cm_t inner_product = static_cast<cm_t>(0);
        for (size_t k = 0; k < N; k++)
        {
            inner_product += vec._coords[k] * _mat[row][k];
        }
        result._coords[row] = inner_product;
    }
    return result;
}

NAMESPACE_LINEAR_ALGEBRA_END