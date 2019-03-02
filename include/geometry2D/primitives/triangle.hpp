#pragma once

#include "../geometry2D_common.hpp"

NAMESPACE_GEOMETRY2D_BEGIN

template<class T>
class triangle
{
private:
    std::array<point_type<T>, 3> _points;
public:
    using calculation_type = T;
public:
    triangle();
    triangle(const T& x1, const T& y1, const T& x2, const T& y2, const T& x3, const T& y3);
    triangle(const point_type<T>& v1, const point_type<T>& v2, const point_type<T>& v3);
    triangle(std::initializer_list<point_type<T>> init_list);
    triangle(std::initializer_list<T> init_list);
public:
    point_type<T>* data();
    const point_type<T>* data() const;
    constexpr size_t size() const;

    point_type<T>& operator[](size_t i);
    const point_type<T>& operator[](size_t i) const;

    auto begin();
    auto begin() const;
    auto end();
    auto end() const;
public:
    point_type<T> center() const;

    point_type<T>& p1();
    const point_type<T>& p1() const;
    void set_p1(const point_type<T>& p);
    point_type<T>& p2();
    const point_type<T>& p2() const;
    void set_p2(const point_type<T>& p);
    point_type<T>& p3();
    const point_type<T>& p3() const;
    void set_p3(const point_type<T>& p);

    template<inclusion_mode mode = inclusion_mode::full>
    bool contains(const point_type<T>& p) const;
public:
    bool operator==(const triangle& other) const;
    bool operator!=(const triangle& other) const;
public:
    template<class T>
    friend std::ostream& operator<<(std::ostream& os, const triangle<T>& t);
};

NAMESPACE_GEOMETRY2D_END