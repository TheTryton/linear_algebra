#pragma once

#include "../console_renderer_common.hpp"
#include "../model/model.hpp"
#include "../surface/surface.hpp"

NAMESPACE_CONSOLE_RENDERER_BEGIN

template<class T, class O>
void draw_point(const geometry::point_type<T, 3>& p, surface<T, O>& s, fragment_shader<T, O> f_shader)
{
    if(p.z() >= 0)
    {
        if (
            p.x() >= static_cast<T>(0) && p.x() <= static_cast<T>(1.0) &&
            p.y() >= static_cast<T>(0) && p.y() <= static_cast<T>(1.0)
            )
        {
            if (s.depth_buffer_enabled())
            {
                auto p_x = p.x() * (s.width() - 1);
                auto p_y = p.y() * (s.height() - 1);

                if (p.z() < s.depth_buffer_value(p_x, p_y))
                {
                    s.set_depth_buffer_value(p_x, p_y, p.z());
                    s[p_x][p_y] = f_shader(p);
                }
            }
        }
    }
}

template<class T, class O>
void draw_segment(const geometry::simplex<double, 2, 3>& segment, surface<T, O>& s)
{
    if (linear_algebra::equal(segment[1][0], segment[0][0]))
    {
        int x = static_cast<int>(std::round(segment[0][0]));
        int min_y = static_cast<int>(std::floor(std::min(segment[0][1], segment[1][1])));
        int max_y = static_cast<int>(std::ceil(std::max(segment[0][1], segment[1][1])));
        min_y = std::max(std::min(min_y, (int)screen_height - 1), 0);
        max_y = std::max(std::min(max_y, (int)screen_height - 1), 0);
        for (int y = min_y; y <= max_y; y++)
        {
            s[x][y] = '#';
        }
    }
    else
    {
        //clip segment to screen rect
        auto intersect = geometry2D::intersection(geometry2D::rect<double>(0, 0, screen_width, screen_height), segment);

        if (intersect)
        {
            auto& result = *intersect;
            if (result.index() == 1)
            {
                auto pts = std::get<1>(result);
                auto clipped_segment = geometry2D::segment(pts[0], pts[1]);

                auto creating_vector = clipped_segment[1] - clipped_segment[0];

                auto lin_fun = [&clipped_segment, &creating_vector](auto x) {
                    return clipped_segment[0][1] + (x - clipped_segment[0][0]) / creating_vector.x() * creating_vector.y();
                };

                int start_x = static_cast<int>(clipped_segment[0][0]);
                int end_x = static_cast<int>(clipped_segment[1][0]);

                for (int x = start_x; x <= end_x; x++)
                {
                    int y = static_cast<int>(lin_fun(x));
                    surface[y*screen_width + x] = '#';
                }
            }
        }
    }
}

template<class T, class D, class O>
void render(const model<T, D>& m, surface<T, O>& s, vertex_shader<T, D> v_shader, fragment_shader<T, O> f_shader)
{
    if(m._indices)
    {

    }
    else
    {

    }
}

NAMESPACE_CONSOLE_RENDERER_END