#pragma once

#include "../geometry2D_common.hpp"

NAMESPACE_GEOMETRY2D_BEGIN

template<class T>
class line
{
    template<class TO>
    friend class segment;
private:
    std::array<point_type<T>, 2> _points;
public:
    using calculation_type = T;
public:
    line();
    line(const T& x1, const T& y1, const T& x2, const T& y2);
    line(const point_type<T>& v1, const point_type<T>& v2);
    line(std::initializer_list<point_type<T>> init_list);
    line(std::initializer_list<T> init_list);
public:
    template<class D>
    line(const line<D>& other);
    template<class D>
    line(line<D>&& other);
    template<class D>
    line& operator=(const line<D>& other);
    template<class D>
    line& operator=(line<D>&& other);

    template<class D>
    line(const segment<D>& other);
    template<class D>
    line(segment<D>&& other);
    template<class D>
    line& operator=(const segment<D>& other);
    template<class D>
    line& operator=(segment<D>&& other);
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

    template<inclusion_mode mode = inclusion_mode::full>
    bool contains(const point_type<T>& p, const T& line_width) const;

    void translate(const T& dx, const T& dy);
    void translate(const vector_type<T>& v);

    line translated(const T& dx, const T& dy) const;
    line translated(const vector_type<T>& v) const;
public:
    bool operator==(const line& other) const;
    bool operator!=(const line& other) const;
public:
    template<class D>
    explicit operator segment<D>() const;
public:
    template<class TO>
    friend std::ostream& operator<<(std::ostream& os, const line<TO>& l);
};

NAMESPACE_GEOMETRY2D_END