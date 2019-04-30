#pragma once

#include "../geometry_common.hpp"

NAMESPACE_GEOMETRY_BEGIN

NAMESPACE_GEOMETRY_IO_BEGIN

template<class TO, size_t SDO, size_t DO>
std::ostream& operator<<(std::ostream& os, const simplex<TO, SDO, DO>& s);

NAMESPACE_GEOMETRY_IO_END

template<class T, size_t SD, size_t D>
class simplex
{
    template<class TO, size_t SDO, size_t DO>
    friend class simplex;
    template<class TO, size_t SDO, size_t DO>
    friend class space;
    static_assert(SD <= D, "Simplex dimension must must less or equal to vector space dimension!");
private:
    std::array<point_type<T, D>, SD + 1> _points;
public:
    using vector_space_field_type = T;
    static constexpr size_t vector_space_dimension = D;
    static constexpr size_t simplex_dimension = SD;
public:
    //default constructor

    simplex();

    //copy, move constructors and operators

    simplex(const simplex<T, SD, D>& other);
    simplex(simplex<T, SD, D>&& other);
    simplex<T, SD, D>& operator=(const simplex<T, SD, D>& other);
    simplex<T, SD, D>& operator=(simplex<T, SD, D>&& other);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    simplex(const simplex<TO, SD, D>& other);
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    simplex(simplex<TO, SD, D>&& other);
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    simplex<T, SD, D>& operator=(const simplex<TO, SD, D>& other);
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    simplex<T, SD, D>& operator=(simplex<TO, SD, D>&& other);

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    simplex(const simplex<TO, SD, DO>& other);
    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    simplex(simplex<TO, SD, DO>&& other);
    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    simplex<T, SD, D>& operator=(const simplex<TO, SD, DO>& other);
    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    simplex<T, SD, D>& operator=(simplex<TO, SD, DO>&& other);

    //regular constructors

    template<class... TOS, typename = typename std::enable_if_t<(std::is_convertible_v<TOS, T> && ...) && sizeof...(TOS) == (SD + 1)*D>>
    simplex(const TOS&... vs);

    template<class... TOS, typename = typename std::enable_if_t<(std::is_convertible_v<TOS, T> && ...) && sizeof...(TOS) == (SD + 1)*D>>
    simplex(TOS&&... vs);

    template<class... TOS, size_t... DOS, typename = typename std::enable_if_t<(std::is_convertible_v<TOS, T> && ...) && sizeof...(TOS) == SD + 1 && ((DOS >= D) && ...)>>
    simplex(const point_type<TOS, DOS>&... vs);

    template<class... TOS, size_t... DOS, typename = typename std::enable_if_t<(std::is_convertible_v<TOS, T> && ...) && sizeof...(TOS) == SD + 1 && ((DOS >= D) && ...)>>
    simplex(point_type<TOS, DOS>&&... vs);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    simplex(std::initializer_list<TO> init_list);

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T> && DO >= D>>
    simplex(std::initializer_list<point_type<TO, DO>> init_list);

    //regular operators

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    simplex<T, SD, D>& operator=(std::initializer_list<TO> init_list);

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T> && DO >= D>>
    simplex<T, SD, D>& operator=(std::initializer_list<point_type<TO, DO>> init_list);

    //space conversion constructors/operators

    simplex(const space<T, SD, D>& other);
    simplex(space<T, SD, D>&& other);
    simplex<T, SD, D>& operator=(const space<T, SD, D>& other);
    simplex<T, SD, D>& operator=(space<T, SD, D>&& other);

    template<class TO>
    simplex(const space<TO, SD, D>& other);
    template<class TO>
    simplex(space<TO, SD, D>&& other);
    template<class TO>
    simplex<T, SD, D>& operator=(const space<TO, SD, D>& other);
    template<class TO>
    simplex<T, SD, D>& operator=(space<TO, SD, D>&& other);

    template<class TO, size_t DO>
    simplex(const space<TO, SD, DO>& other);
    template<class TO, size_t DO>
    simplex(space<TO, SD, DO>&& other);
    template<class TO, size_t DO>
    simplex<T, SD, D>& operator=(const space<TO, SD, DO>& other);
    template<class TO, size_t DO>
    simplex<T, SD, D>& operator=(space<TO, SD, DO>&& other);
public:
    //simplex info and accessors

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
    //simplex methods

    point_type<T, D> center() const;

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

    template<inclusion_mode mode = inclusion_mode::full>
    bool contains(const point_type<T, D>& p) const;
public:
    template<class TO, size_t SDO, size_t DO>
    friend std::ostream& GEOMETRY_IO::operator<<(std::ostream& os, const simplex<TO, SDO, DO>& s);
};

NAMESPACE_GEOMETRY_END