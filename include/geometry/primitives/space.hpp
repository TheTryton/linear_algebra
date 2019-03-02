#pragma once

#include "../geometry_common.hpp"

NAMESPACE_GEOMETRY_BEGIN

NAMESPACE_GEOMETRY_IO_BEGIN

template<class TO, size_t SDO, size_t DO>
std::ostream& operator<<(std::ostream& os, const space<TO, SDO, DO>& s);

NAMESPACE_GEOMETRY_IO_END

template<class T, size_t SD, size_t D>
class space
{
    template<class T, size_t SD, size_t D>
    friend class space;
    template<class T, size_t SD, size_t D>
    friend class simplex;
    static_assert(SD <= D, "Space dimension must must less or equal to vector space dimension!");
private:
    std::array<point_type<T, D>, SD + 1> _points;
public:
    using vector_space_field_type = T;
    static constexpr size_t vector_space_dimension = D;
    static constexpr size_t space_dimension = SD;
public:
    //default constructor

    space();

    //copy, move constructors and operators

    space(const space<T, SD, D>& other);
    space(space<T, SD, D>&& other);
    space<T, SD, D>& operator=(const space<T, SD, D>& other);
    space<T, SD, D>& operator=(space<T, SD, D>&& other);

    template<class TO>
    space(const space<TO, SD, D>& other);
    template<class TO>
    space(space<TO, SD, D>&& other);
    template<class TO>
    space<T, SD, D>& operator=(const space<TO, SD, D>& other);
    template<class TO>
    space<T, SD, D>& operator=(space<TO, SD, D>&& other);

    template<class TO, size_t DO>
    space(const space<TO, SD, DO>& other);
    template<class TO, size_t DO>
    space(space<TO, SD, DO>&& other);
    template<class TO, size_t DO>
    space<T, SD, D>& operator=(const space<TO, SD, DO>& other);
    template<class TO, size_t DO>
    space<T, SD, D>& operator=(space<TO, SD, DO>&& other);

    //regular constructors

    template<class... TOS, typename = typename std::enable_if_t<std::conjunction_v<std::is_convertible<TOS, T>...> && sizeof...(TOS) >= (SD + 1)*D>>
    space(const TOS&... vs);

    template<class... TOS, typename = typename std::enable_if_t<std::conjunction_v<std::is_convertible<TOS, T>...> && sizeof...(TOS) >= (SD + 1)*D>>
    space(TOS&&... vs);
    
    template<class... TOS, size_t... DOS, typename = typename std::enable_if_t<std::conjunction_v<std::is_convertible<TOS, T>...> && sizeof...(TOS) >= SD + 1 && ((DOS >= D) && ...)>>
    space(const point_type<TOS, DOS>&... vs);

    template<class... TOS, size_t... DOS, typename = typename std::enable_if_t<std::conjunction_v<std::is_convertible<TOS, T>...> && sizeof...(TOS) >= SD + 1 && ((DOS >= D) && ...)>>
    space(point_type<TOS, DOS>&&... vs);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    space(std::initializer_list<TO> init_list);

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T> && DO >= D>>
    space(std::initializer_list<point_type<TO, DO>> init_list);

    //regular operators

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    space<T, SD, D>& operator=(std::initializer_list<TO> init_list);

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T> && DO >= D>>
    space<T, SD, D>& operator=(std::initializer_list<point_type<TO, DO>> init_list);

    // simpex conversion constructors/operators

    space(const simplex<T, SD, D>& other);
    space(simplex<T, SD, D>&& other);
    space<T, SD, D>& operator=(const simplex<T, SD, D>& other);
    space<T, SD, D>& operator=(simplex<T, SD, D>&& other);

    template<class TO>
    space(const simplex<TO, SD, D>& other);
    template<class TO>
    space(simplex<TO, SD, D>&& other);
    template<class TO>
    space<T, SD, D>& operator=(const simplex<TO, SD, D>& other);
    template<class TO>
    space<T, SD, D>& operator=(simplex<TO, SD, D>&& other);

    template<class TO, size_t DO>
    space(const simplex<TO, SD, DO>& other);
    template<class TO, size_t DO>
    space(simplex<TO, SD, DO>&& other);
    template<class TO, size_t DO>
    space<T, SD, D>& operator=(const simplex<TO, SD, DO>& other);
    template<class TO, size_t DO>
    space<T, SD, D>& operator=(simplex<TO, SD, DO>&& other);
public:
    //vector info and accessors

    point_type<T, D>* data();
    const point_type<T, D>* data() const;

    constexpr size_t size();

    point_type<T, D>& operator[](size_t i);
    const point_type<T, D>& operator[](size_t i) const;

    auto begin();
    auto begin() const;
    auto end();
    auto end() const;
public:
    //space methods

    point_type<T, D>& p1();
    const point_type<T, D>& p1() const;
    void set_p1(const point_type<T, D>& p);

    template<typename = typename std::enable_if_t<SD >= 1>>
    point_type<T, D>& p2();
    template<typename = typename std::enable_if_t<SD >= 1>>
    const point_type<T, D>& p2() const;
    template<typename = typename std::enable_if_t<SD >= 1>>
    void set_p2(const point_type<T, D>& p);

    template<typename = typename std::enable_if_t<SD >= 2>>
    point_type<T, D>& p3();
    template<typename = typename std::enable_if_t<SD >= 2>>
    const point_type<T, D>& p3() const;
    template<typename = typename std::enable_if_t<SD >= 2>>
    void set_p3(const point_type<T, D>& p);

    template<typename = typename std::enable_if_t<SD >= 3>>
    point_type<T, D>& p4();
    template<typename = typename std::enable_if_t<SD >= 3>>
    const point_type<T, D>& p4() const;
    template<typename = typename std::enable_if_t<SD >= 3>>
    void set_p4(const point_type<T, D>& p);

    template<typename = typename std::enable_if_t<SD >= 4>>
    point_type<T, D>& p5();
    template<typename = typename std::enable_if_t<SD >= 4>>
    const point_type<T, D>& p5() const;
    template<typename = typename std::enable_if_t<SD >= 4>>
    void set_p5(const point_type<T, D>& p);
public:
    template<class TO, size_t SDO, size_t DO>
    friend std::ostream& GEOMETRY_IO::operator<<(std::ostream& os, const space<TO, SDO, DO>& s);
};

NAMESPACE_GEOMETRY_END