#pragma once

#include "../geometry2D_common.hpp"

NAMESPACE_GEOMETRY2D_BEGIN

template<class T>
class rect
{
private:
    std::array<point_type<T>, 2> _points;
public:
    using calculation_type = T;
public:
    rect();
    rect(const point_type<T>& bottom_left, const point_type<T>& top_right);
    rect(const T& x, const T& y, const T& w, const T& h);
    rect(std::initializer_list<point_type<T>> init_list);
    rect(std::initializer_list<T> init_list);

    template<class D>
    rect(const rect<D>& other);
    template<class D>
    rect(rect<D>&& other);
    template<class D>
    rect& operator=(const rect<D>& other);
    template<class D>
    rect& operator=(rect<D>&& other);
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
    void adjust(const T& dlx, const T& duy, const T& drx, const T& ddy);
    rect adjusted(const T& dlx, const T& duy, const T& drx, const T& ddy) const;

    T& left();
    const T& left() const;
    void set_left(const T& v);
    T& right();
    const T& right() const;
    void set_right(const T& v);
    T& bottom();
    const T& bottom() const;
    void set_bottom(const T& v);
    T& top();
    const T& top() const;
    void set_top(const T& v);

    point_type<T> bottom_left() const;
    void set_bottom_left(const point_type<T>& p);
    point_type<T> bottom_right() const;
    void set_bottom_right(const point_type<T>& p);
    point_type<T> top_left() const;
    void set_top_left(const point_type<T>& p);
    point_type<T> top_right() const;
    void set_top_right(const point_type<T>& p);

    point_type<T> center() const;

    template<inclusion_mode mode = inclusion_mode::full>
    bool contains(const point_type<T>& p) const;
    template<inclusion_mode mode = inclusion_mode::full>
    bool contains(const rect<T>& r) const;

    T& x();
    const T& x() const;
    void set_x(const T& v);
    T& y();
    const T& y() const;
    void set_y(const T& v);

    T width() const;
    void set_width(const T& w);
    T height() const;
    void set_height(const T& h);

    bool intersects(const rect<T>& r) const;
    std::optional<rect<T>> intersection(const rect<T>& r) const;

    void normalize();
    rect<T> normalized() const;

    void translate(const T& dx, const T& dy);
    void translate(const vector_type<T>& v);

    rect<T> translated(const T& dx, const T& dy) const;
    rect<T> translated(const vector_type<T>& v) const;

    rect<T> united(const rect<T>& r) const;
public:
    bool operator==(const rect& other) const;
    bool operator!=(const rect& other) const;
public:
    template<class T>
    friend std::ostream& operator<<(std::ostream& os, const rect<T>& rect);
};

NAMESPACE_GEOMETRY2D_END