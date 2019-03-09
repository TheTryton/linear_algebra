#pragma once

#include "../geometry_common.hpp"
#include "../primitives/space.inl"

NAMESPACE_GEOMETRY_BEGIN

namespace detail
{
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
}

template<class T, size_t D, size_t K>
using perpendicular_result = typename detail::perpendicular_result_impl <
    T,
    D,
    decltype(std::make_index_sequence<K>())
>::perpendicular_result_type;

template<class T, size_t D, class... TOS, typename = typename std::enable_if_t<(sizeof...(TOS) + 1 < D)>>
std::optional<perpendicular_result<T, D, sizeof...(TOS) + 1>> perpendicular(const vector_type<T, D>& v, const vector_type<TOS, D>&... vs);

template<class T, size_t D, size_t K>
std::optional<perpendicular_result<T, D, K>> perpendicular(const std::array<vector_type<T, D>, K>& vectors);

NAMESPACE_GEOMETRY_END