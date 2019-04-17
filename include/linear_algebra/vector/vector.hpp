#pragma once

#include "../linear_algebra_common_functions.hpp"

NAMESPACE_LINEAR_ALGEBRA_BEGIN

NAMESPACE_LINEAR_ALGEBRA_IO_BEGIN

template<class TO, size_t DO>
std::ostream& operator<<(std::ostream& os, const vector<TO, DO>& v);

NAMESPACE_LINEAR_ALGEBRA_IO_END

template<class T, size_t D>
class vector
{
    static_assert(D != 0 , "Vector dimension must be at least 1!");
    static_assert(is_valid_mathematical_field_v<T>, "Vector element type must satisfy valid_mathematical_field concept!");
    
    template<class TO, size_t DO>
    friend class vector;
    template<class T, size_t N, size_t M>
    friend class matrix;
private:
    class vector_storage_static
    {
    private:
        T _coords[D];
    public:
        inline T& operator[](size_t d);
        inline const T& operator[](size_t d) const;

        inline T* data();
        inline const T* data() const;

        inline T* begin();
        inline const T* begin() const;

        inline T* end();
        inline const T* end() const;
    };

    class vector_storage_dynamic
    {
    private:
        T* _coords = nullptr;
    public:
        inline vector_storage_dynamic();
        inline vector_storage_dynamic(vector<T, D>::vector_storage_dynamic&& other);
        inline vector<T, D>::vector_storage_dynamic& operator=(vector<T, D>::vector_storage_dynamic&& other);
        inline ~vector_storage_dynamic();
    public:
        inline T& operator[](size_t d);
        inline const T& operator[](size_t d) const;

        inline T* data();
        inline const T* data() const;

        inline T* begin();
        inline const T* begin() const;

        inline T* end();
        inline const T* end() const;
    };

    static constexpr bool is_big_vector = D * sizeof(T) >= static_storage_max_size;

    using storage_type = typename std::conditional_t<is_big_vector, vector_storage_dynamic, vector_storage_static>;
    
    storage_type _coords;
public:
    using mathematical_field_type = T;

    static constexpr size_t size = D;
    static constexpr size_t dimension = D;
public:
    //default constructor

    vector();
    
    //copy, move constructors and operators

    //same type, same dimensions

    vector(const vector<T, D>& other);

    vector(vector<T, D>&& other);

    vector<T, D>& operator=(const vector<T, D>& other);
    
    vector<T, D>& operator=(vector<T, D>&& other);

    //different type, same dimensions

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector(const vector<TO, D>& other);
    
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector(vector<TO, D>&& other);
    
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator=(const vector<TO, D>& other);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator=(vector<TO, D>&& other);

    //different type, different dimensions

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector(const vector<TO, DO>& other);

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector(vector<TO, DO>&& other);

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator=(const vector<TO, DO>& other);

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator=(vector<TO, DO>&& other);

    //regular constructors
    
    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector(const TO& v);

    template<class... TOS, typename = typename std::enable_if_t<(sizeof...(TOS) > 1) && (std::is_convertible_v<TOS, T> && ...)>>
    vector(const TOS&... vs);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector(TO&& v);
    
    template<class... TOS, typename = typename std::enable_if_t<(sizeof...(TOS) > 1) && (std::is_convertible_v<TOS, T> && ...)>>
    vector(TOS&&... vs);
    
    vector(std::initializer_list<T> init_list);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector(std::initializer_list<TO> init_list);

    //regular operators

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator=(const TO& v);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator=(TO&& v);
    
    vector<T, D>& operator=(std::initializer_list<T> init_list);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator=(std::initializer_list<TO> init_list);
public:
    //vector-vector operators

    template<class TO, size_t DO, typename = typename std::enable_if_t<can_be_added_v<T, TO>>>
    vector<addition_result_t<T, TO>, smaller<D, DO>> operator+(const vector<TO, DO>& other) const;
    
    template<class TO, size_t DO, typename = typename std::enable_if_t<can_be_subtracted_v<T, TO>>>
    vector<subtraction_result_t<T, TO>, smaller<D, DO>> operator-(const vector<TO, DO>& other) const;
    
    //shortcut for inner product
    template<class TO, size_t DO, typename = typename std::enable_if_t<can_calculate_inner_product_v<T, TO>>>
    inner_product_result_t<T, TO> operator*(const vector<TO, DO>& other) const; 

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator+=(const vector<TO, DO>& other);

    template<class TO, size_t DO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator-=(const vector<TO, DO>& other);

    //vector-scalar operators

    vector<T, D> operator-() const;

    template<class TO, typename = typename std::enable_if_t<can_be_multiplied_v<T, TO> && std::is_convertible_v<TO, T>>>
    vector<multiplication_result_t<T, TO>, D> operator*(const TO& v) const;
    
    template<class TO, typename = typename std::enable_if_t<can_be_divided_v<T, TO> && std::is_convertible_v<TO, T>>>
    vector<division_result_t<T, TO>, D> operator/(const TO& v) const;

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator*=(const TO& v);

    template<class TO, typename = typename std::enable_if_t<std::is_convertible_v<TO, T>>>
    vector<T, D>& operator/=(const TO& v);
public:
    //vector info and accessors

    T& operator[](size_t d);
    const T& operator[](size_t d) const;

    T* data();
    const T* data() const;

    T* begin();
    const T* begin() const;

    T* end();
    const T* end() const;
public:
    //comparison operators

    template<class TO, size_t DO, typename = typename std::enable_if_t<use_high_quality_equality_comparison ? high_quality_equality_comparable<T1, T2> || equality_comparable_v<T1, T2 > : equality_comparable_v<T1, T2>>>
    bool operator==(const vector<TO, DO>& other) const;
    
    template<class TO, size_t DO, typename = typename std::enable_if_t<use_high_quality_inequality_comparison ? high_quality_inequality_comparable<T1, T2> || inequality_comparable_v<T1, T2 > : inequality_comparable_v<T1, T2>>>
    bool operator!=(const vector<TO, DO>& other) const;
public:
    template<typename = typename std::enable_if_t<D >= 1 >>
    T& x();
    template<typename = typename std::enable_if_t<D >= 1>>
    const T& x() const;
    template<typename = typename std::enable_if_t<D >= 1>>
    void set_x(const T& x);
    
    template<typename = typename std::enable_if_t<D >= 2>>
    T& y();
    template<typename = typename std::enable_if_t<D >= 2>>
    const T& y() const;
    template<typename = typename std::enable_if_t<D >= 2>>
    void set_y(const T& y);
    
    template<typename = typename std::enable_if_t<D >= 3>>
    T& z();
    template<typename = typename std::enable_if_t<D >= 3>>
    const T& z() const;
    template<typename = typename std::enable_if_t<D >= 3>>
    void set_z(const T& z);
    
    template<typename = typename std::enable_if_t<D >= 4>>
    T& w();
    template<typename = typename std::enable_if_t<D >= 4>>
    const T& w() const;
    template<typename = typename std::enable_if_t<D >= 4>>
    void set_w(const T& w);

    template<typename = typename std::enable_if_t<D == 2>>
    vector<T, D> perpendicularLH() const;
    template<typename = typename std::enable_if_t<D == 2>>
    vector<T, D> perpendicularRH() const;

    template<class TO, typename = typename std::enable_if_t<D == 3>>
    vector<T, D> cross_productLH(const vector<TO, D>& other) const;
    template<class TO, typename = typename std::enable_if_t<D == 3>>
    vector<T, D> cross_productRH(const vector<TO, D>& other) const;

    template<typename = typename std::enable_if_t<has_sqrt_implementation_v<T>>>
    T magnitude() const;
    template<typename = typename std::enable_if_t<has_sqrt_implementation_v<T>>>
    T length() const;

    T magnitude_sqr() const;
    T length_sqr() const;

    template<typename = typename std::enable_if_t<has_sqrt_implementation_v<T>>>
    T normalized() const;
    template<typename = typename std::enable_if_t<has_sqrt_implementation_v<T>>>
    vector<T, D>& normalize();
    
    template<class TO, typename = typename std::enable_if_t<can_calculate_inner_product_v<T, TO>>>
    inner_product_result_t<T, TO> inner_product(const vector<TO, D>& other) const;
public:
    template<class TO, size_t DO>
    friend std::ostream& LINEAR_ALGEBRA_IO::operator<<(std::ostream& os, const vector<TO, DO>& v);
public:
    static const vector<T, D>& zero();
    static const vector<T, D>& one();
};

NAMESPACE_LINEAR_ALGEBRA_END