#pragma once

#include "../geometry/geometry_common.hpp"
#include <cassert>
#include <vector>
#include <functional>

#define CONSOLE_RENDERER console_renderer
#define NAMESPACE_CONSOLE_RENDERER_BEGIN namespace CONSOLE_RENDERER{
#define NAMESPACE_CONSOLE_RENDERER_END }

NAMESPACE_CONSOLE_RENDERER_BEGIN

enum primitive_topology
{
    point_list,
    line_list,
    line_strip,
    line_loop,
    triangle_list,
    triangle_strip,
    triangle_fan,
};

template<class T, class O>
class surface;

template<class T, class D>
class model;

template<class T, class D>
using vertex_shader = std::function<geometry::point_type<T, 3>(geometry::point_type<T, 3>, D)>;

template<class T, class O>
using fragment_shader = std::function<O(geometry::point_type<T, 3>)>;

template<class T, class D, class O>
void render(const model<T, D>& m, surface<T, O>& s, vertex_shader<T,D> v_shader, fragment_shader<T,O> f_shader);

NAMESPACE_CONSOLE_RENDERER_END