#pragma once

#include "../geometry_common.hpp"

NAMESPACE_GEOMETRY_BEGIN

NAMESPACE_GEOMETRY_IO_BEGIN

template<class TO, size_t DO>
std::ostream& operator<<(std::ostream& os, const sphere<TO, DO>& s);

NAMESPACE_GEOMETRY_IO_END

template<class T, size_t D>
class sphere
{
private:
    point_type<T, D> _center;
    T _radius;
public:
    using vector_space_field_type = T;
    static constexpr size_t vector_space_dimension = D;
    static constexpr size_t sphere_dimension = D;
public:
    //default constructor

    sphere();

    //copy, move constructors and operators

    sphere(const sphere<T, D>& other);
    sphere(sphere<T, D>&& other);
    sphere<T, D>& operator=(const sphere<T, D>& other);
    sphere<T, D>& operator=(sphere<T, D>&& other);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    sphere(const sphere<TO, D>& other);
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    sphere(sphere<TO, D>&& other);
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    sphere<T, D>& operator=(const sphere<TO, D>& other);
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    sphere<T, D>& operator=(sphere<TO, D>&& other);

    //regular constructors

    template<class TO1, class TO2, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO1, T> && std::is_convertible_v<TO2, T> && DO >= D>>
    sphere(const point_type<TO1, D>& center, const TO2& radius);

    template<class... TOS, typename = typename std::enable_if_t<(std::is_convertible_v<TOS, T> && ...) && sizeof...(TOS) == (D + 1)*D>>
    sphere(const TOS&... vs);

    template<class... TOS, typename = typename std::enable_if_t<(std::is_convertible_v<TOS, T> && ...) && sizeof...(TOS) == (D + 1)*D>>
    sphere(TOS&&... vs);

    template<class... TOS, size_t... DOS, typename = typename std::enable_if_t<(std::is_convertible_v<TOS, T> && ...) && sizeof...(TOS) == D + 1 && ((DOS >= D) && ...)>>
    sphere(const point_type<TOS, DOS>&... vs);

    template<class... TOS, size_t... DOS, typename = typename std::enable_if_t<(std::is_convertible_v<TOS, T> && ...) && sizeof...(TOS) == D + 1 && ((DOS >= D) && ...)>>
    sphere(point_type<TOS, DOS>&&... vs);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    sphere(std::initializer_list<TO> init_list);

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T> && DO >= D>>
    sphere(std::initializer_list<point_type<TO, DO>> init_list);

    //regular operators

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    sphere<T, D>& operator=(std::initializer_list<TO> init_list);

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T> && DO >= D>>
    sphere<T, D>& operator=(std::initializer_list<point_type<TO, DO>> init_list);
public:
    //sphere methods

    point_type<T, D>& center();
    const point_type<T, D>& center() const;
    void set_center(const point_type<T, D>& center);

    T& radius();
    const T& radius() const;
    void set_radius(const T& radius);

    template<inclusion_mode mode = inclusion_mode::full>
    bool contains(const point_type<T, D>& p) const;
public:
    template<class TO, size_t DO>
    friend std::ostream& GEOMETRY_IO::operator<<(std::ostream& os, const sphere<TO, DO>& s);
};

NAMESPACE_GEOMETRY_END