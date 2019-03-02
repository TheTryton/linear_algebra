#pragma once

#include "../console_renderer_common.hpp"

NAMESPACE_CONSOLE_RENDERER_BEGIN

template<class T, class D>
class model
{
    static_assert(std::is_default_constructible_v<D>, "Surface pixel type must be default constructible!");
private:
    std::vector<std::pair<geometry::point_type<T, 3>, D>> _vertices;
    std::optional<std::vector<size_t>> _indices;
    primitive_topology _topology;
public:
    model():
        _topology(primitive_topology::point_list)
    {
    }
    template<class TO, class DO>
    model(const model<TO, DO>&) = delete;
    template<class TO, class DO>
    model(model<TO, DO>&&) = delete;
    template<class TO, class DO>
    model& operator=(const model<TO, DO>&) = delete;
    template<class TO, class DO>
    model& operator=(model<TO, DO>&&) = delete;
public:
    void set_vertices_count(size_t count)
    {
        _vertices.resize(count);
    }
    size_t vertices_count() const
    {
        return _vertices.size();
    }

    void set_indices_count(size_t count)
    {
        if(count)
        {
            _indices = std::vector<size_t>(count);
        }
        else
        {
            _indices = std::nullopt;
        }
    }
    size_t indices_count() const
    {
        if(_indices)
        {
            return _indices->size();
        }
        return 0;
    }

    void set_indices(const std::vector<size_t>& indices_buffer)
    {
        _indices = indices_buffer;
    }
    void set_vertices(const std::vector<std::pair<geometry::point_type<T, 3>, D>>& vertices)
    {
        _vertices = vertices;
    }

    primitive_topology primitive_topology() const
    {
        return _topology;
    }
    void set_primitive_topology(const CONSOLE_RENDERER::primitive_topology& topology)
    {
        _topology = topology;
    }
public:
    template<class T, class D, class O>
    void render(const model<T, D>& m, surface<T, O>& s, vertex_shader<T, D> v_shader, fragment_shader<T, O> f_shader);
};

NAMESPACE_CONSOLE_RENDERER_END