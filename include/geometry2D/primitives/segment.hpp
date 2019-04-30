#pragma once

#include "../geometry2D_common.hpp"

NAMESPACE_GEOMETRY2D_BEGIN

template<class T>
class segment
{
    template<class TO>
    friend class line;
private:
    std::array<point_type<T>, 2> _points;
public:
    using calculation_type = T;
public:
    segment();
    segment(const T& x1, const T& y1, const T& x2, const T& y2);
    segment(const point_type<T>& v1, const point_type<T>& v2);
    segment(std::initializer_list<point_type<T>> init_list);
    segment(std::initializer_list<T> init_list);
public:
    template<class D>
    segment(const segment<D>& other);
    template<class D>
    segment(segment<D>&& other);
    template<class D>
    segment& operator=(const segment<D>& other);
    template<class D>
    segment& operator=(segment<D>&& other);

    template<class D>
    segment(const line<D>& other);
    template<class D>
    segment(line<D>&& other);
    template<class D>
    segment& operator=(const line<D>& other);
    template<class D>
    segment& operator=(line<D>&& other);
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

    point_type<T> evaluate(const T& x) const;

    point_type<T>& p1();
    const point_type<T>& p1() const;
    void set_p1(const point_type<T>& p);
    point_type<T>& p2();
    const point_type<T>& p2() const;
    void set_p2(const point_type<T>& p);

    T length() const;

    template<inclusion_mode mode = inclusion_mode::full>
    bool contains(const point_type<T>& p, const T& segment_width) const;

    void translate(const T& dx, const T& dy);
    void translate(const vector_type<T>& v);
    segment translated(const T& dx, const T& dy) const;
    segment translated(const vector_type<T>& v) const;
public:
    bool operator==(const segment& other) const;
    bool operator!=(const segment& other) const;
public:
    template<class D>
    explicit operator line<D>() const;
public:
    template<class TO>
    friend std::ostream& operator<<(std::ostream& os, const segment<T>& s);
};

NAMESPACE_GEOMETRY2D_END