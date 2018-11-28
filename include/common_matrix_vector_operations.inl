#pragma once

#include "vector/vector.hpp"
#include "matrix/matrix.hpp"

template<class T, uint D>
template<class D0, uint M>
auto vector<T, D>::operator*(const matrix<D0, D, M>& matrix) const
{
    using cm_t = decltype(std::declval<T>()*std::declval<D0>() + std::declval<T>()*std::declval<D0>());
    vector<cm_t, M> result;
    for (uint y = 0; y < M; y++)
    {
        cm_t inner_product = static_cast<cm_t>(0);
        for (uint k = 0; k < D; k++)
        {
            inner_product += _coords[k] * matrix._mat[k][y];
        }
        result._coords[y] = inner_product;
    }
    return result;
}

template<class T, uint D>
template<class D0>
vector<T, D>& vector<T, D>::operator*=(const matrix<D0, D, D>& matrix) const
{
    *this = operator*(matrix);
    return *this;
}

template<class T, uint N, uint M>
template<class D0>
auto matrix<T, N, M>::operator*(const vector<D0, N>& vec) const
{
    using cm_t = decltype(std::declval<T>()*std::declval<D0>() + std::declval<T>()*std::declval<D0>());
    vector<cm_t, M> result;
    for (uint y = 0; y < M; y++)
    {
        cm_t inner_product = static_cast<cm_t>(0);
        for (uint k = 0; k < N; k++)
        {
            inner_product += vec._coords[k] * _mat[k][y];
        }
        result._coords[y] = inner_product;
    }
    return result;
}