#pragma once

#include "../console_renderer_common.hpp"

NAMESPACE_CONSOLE_RENDERER_BEGIN

template<class T,class O>
class surface
{
    static_assert(std::is_default_constructible_v<O>, "Surface pixel type must be default constructible!");
    static_assert(std::is_default_constructible_v<T>, "Surface depth buffer type must be default constructible!");
private:
    O* _surface;
    T* _depth_buffer = nullptr;
    size_t _width;
    size_t _height;
public:
    surface() = delete;
    template<class OO>
    surface(const surface<OO>&) = delete;
    template<class OO>
    surface(surface<OO>&&) = delete;
    template<class OO>
    surface& operator=(const surface<OO>&) = delete;
    template<class OO>
    surface& operator=(surface<OO>&&) = delete;
public:
    surface(size_t width, size_t height):
        _width(width),
        _height(height)
    {
        _surface = new O[_width * _height];

        assert(_width * _height);
    }
public:
    size_t width() const
    {
        return _width;
    }
    size_t height() const
    {
        return _height;
    }

    O* operator[](size_t x)
    {
        return x * _height;
    }
    const O* operator[](size_t x) const
    {
        return x * _height;
    }

    O* data()
    {
        return _surface;
    }
    const O* data() const
    {
        return _surface;
    }

    O* begin()
    {
        return _surface;
    }
    const O* begin() const
    {
        return _surface;
    }
    O* end()
    {
        return _surface + _width * _height;
    }
    const O* end() const
    {
        return _surface + _width * _height;
    }
public:
    void resize(size_t new_width, size_t new_height)
    {
        _width = new_width;
        _height = new_height;
        delete[] _surface;
        _surface = new O[_width * _height];
        if(_depth_buffer)
        {
            delete _depth_buffer;
            _depth_buffer = new T[_width * _height];
        }
    }

    void enable_depth_buffer(bool enable)
    {
        if(_depth_buffer && !enable)
        {
            delete[] _depth_buffer;
            _depth_buffer = nullptr;
        }
        else if(!_depth_buffer && enable)
        {
            _depth_buffer = new T[_width * _height];
        }
    }
    bool depth_buffer_enabled() const
    {
        return _depth_buffer;
    }

    const T& depth_buffer_value(size_t x, size_t y) const
    {
        return _depth_buffer[x*_height + y];
    }
    void set_depth_buffer_value(size_t x, size_t y, const T& value) const
    {
        _depth_buffer[x*_height + y] = value;
    }
};

NAMESPACE_CONSOLE_RENDERER_END