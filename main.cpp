#include "include/linear_algebra/linear_algebra.hpp"
#include "include/geometry2D/geometry2D.hpp"
#include "include/geometry/primitives/simplex.hpp"
#include "include/geometry/primitives/simplex.inl"
#include "include/geometry/primitives/sphere.hpp"
#include "include/geometry/primitives/sphere.inl"
#include "include/geometry/projections/projections.hpp"
#include "include/geometry/projections/projections.inl"
#include "include/geometry/intersections/intersections.hpp"
#include "include/geometry/intersections/intersections.inl"
#include <iostream>
#include <iomanip>
#include <vector>
#include <Windows.h>
#include <chrono>
#include <sstream>
#include <vector>
#undef min
#undef max

#define M_PI 3.14159265358979323846

namespace geometry3D_renderer
{
    using point3D = geometry::point_type<double, 3>;

    using matrix3x3 = linear_algebra::matrix<double, 3, 3>;

    matrix3x3 rotationX(const double& angle)
    {
        auto c = cos(angle);
        auto s = sin(angle);
        return matrix3x3({
            {1.0,0.0,0.0},
            {0.0,c,-s},
            {0.0,s,c}
        });
    }

    matrix3x3 rotationY(const double& angle)
    {
        auto c = cos(angle);
        auto s = sin(angle);
        return matrix3x3({
            {c,0.0,s},
            {0.0,1.0,0.0},
            {-s,0.0,c}
        });
    }

    matrix3x3 rotationZ(const double& angle)
    {
        auto c = cos(angle);
        auto s = sin(angle);
        return matrix3x3({
            {c,-s,0.0},
            {s,c,0.0},
            {0.0,0.0,1.0}
        });
    }

    void draw_triangle(const geometry::triangle<double, 3>& tri, std::vector<char>& surface, size_t width, size_t height)
    {
        auto x_min = std::min({ tri[0][0], tri[1][0], tri[2][0] });
        auto x_max = std::max({ tri[0][0], tri[1][0], tri[2][0] });
        auto y_min = std::min({ tri[0][1], tri[1][1], tri[2][1] });
        auto y_max = std::max({ tri[0][1], tri[1][1], tri[2][1] });

        if (x_min > width - 1 || x_max < 0 || y_min > height - 1 || y_max < 0)
            return;

        uint32_t x_s = std::max(int32_t(0), (int32_t)(std::floor(x_min)));
        uint32_t x_e = std::min(int32_t(width) - 1, (int32_t)(std::floor(x_max)));
        uint32_t y_s = std::max(int32_t(0), (int32_t)(std::floor(y_min)));
        uint32_t y_e = std::min(int32_t(height) - 1, (int32_t)(std::floor(y_max)));

        auto tri2D = geometry::triangle<double, 2>(tri);

        for (size_t x = x_s; x <= x_e; x++)
        {
            for (size_t y = y_s; y <= y_e; y++)
            {
                point3D p = point3D(x, y);
                if (tri2D.contains(p))
                {
                    auto& p1 = tri[0];
                    auto& p2 = tri[1];
                    auto& p3 = tri[2];

                    //BARCENTRIC
                    auto w1 =
                        ((p2[1] - p3[1]) * (p[0] - p3[0]) + (p3[0] - p2[0]) * (p[1] - p3[1])) /
                        ((p2[1] - p3[1]) * (p1[0] - p3[0]) + (p3[0] - p2[0]) * (p1[1] - p3[1]));
                    auto w2 =
                        ((p3[1] - p1[1]) * (p[0] - p3[0]) + (p1[0] - p3[0]) * (p[1] - p3[1])) /
                        ((p2[1] - p3[1]) * (p1[0] - p3[0]) + (p3[0] - p2[0]) * (p1[1] - p3[1]));
                    auto w3 = 1.0f - w1 - w2;

                    auto color = (tri[0][2] * w1 + tri[1][2] * w2 + tri[2][2] * w3) / (w1 + w2 + w3);

                    if (color < 1)
                    {
                        surface[y*width + x] = '#';
                    }
                    else if (color < 3)
                    {
                        if(surface[y*width + x] != '#')
                        {
                            surface[y*width + x] = char(176);
                        }
                    }
                    else
                    {
                        if (surface[y*width + x] != '#' && surface[y*width + x] != char(176))
                        {
                            surface[y*width + x] = '.';
                        }
                    }
                }
            }
        }
    }

    void draw_cube(const point3D& center, const point3D& size, const point3D& rotation, std::vector<char>& surface, size_t width, size_t height)
    {
        std::array<point3D, 8> vertices;

        vertices[0] = point3D(0, -1, 0);
        vertices[1] = point3D(1, -1, 0);
        vertices[2] = point3D(1, -1, 1);
        vertices[3] = point3D(0, -1, 1);

        vertices[4] = vertices[0] + point3D(0, 2, 0);
        vertices[5] = vertices[1] + point3D(0, 2, 0);
        vertices[6] = vertices[2] + point3D(0, 2, 0);
        vertices[7] = vertices[3] + point3D(0, 2, 0);

        auto rot_mat = rotationX(rotation.x())*rotationY(rotation.y())*rotationZ(rotation.z());

        for(auto& v :vertices)
        {
            v = rot_mat * v;
            v[0] *= size[0];
            v[1] *= size[1];
            v[2] *= size[2];
            v += center;
        }

        using triangle = geometry::triangle<double, 3>;
        //bottom
        draw_triangle(triangle(vertices[0], vertices[1], vertices[2]), surface, width, height);
        draw_triangle(triangle(vertices[2], vertices[3], vertices[0]), surface, width, height);
        //top
        draw_triangle(triangle(vertices[4], vertices[5], vertices[6]), surface, width, height);
        draw_triangle(triangle(vertices[6], vertices[7], vertices[4]), surface, width, height);
        //left
        draw_triangle(triangle(vertices[0], vertices[3], vertices[7]), surface, width, height);
        draw_triangle(triangle(vertices[0], vertices[4], vertices[7]), surface, width, height);
        //right
        draw_triangle(triangle(vertices[1], vertices[2], vertices[6]), surface, width, height);
        draw_triangle(triangle(vertices[1], vertices[5], vertices[6]), surface, width, height);
        //front
        draw_triangle(triangle(vertices[0], vertices[1], vertices[4]), surface, width, height);
        draw_triangle(triangle(vertices[1], vertices[5], vertices[3]), surface, width, height);
        //back
        draw_triangle(triangle(vertices[2], vertices[3], vertices[7]), surface, width, height);
        draw_triangle(triangle(vertices[2], vertices[6], vertices[7]), surface, width, height);
    }
}

using point2D = geometry2D::point_type<double>;

constexpr size_t screen_width = 120;
constexpr size_t screen_height = 80;

void draw_triangle(const geometry2D::triangle<double>& tri, std::vector<char>& surface)
{
    auto x_min = std::min({ tri[0][0], tri[1][0], tri[2][0] });
    auto x_max = std::max({ tri[0][0], tri[1][0], tri[2][0] });
    auto y_min = std::min({ tri[0][1], tri[1][1], tri[2][1] });
    auto y_max = std::max({ tri[0][1], tri[1][1], tri[2][1] });

    if (x_min > screen_width - 1 || x_max < 0 || y_min > screen_height - 1 || y_max < 0)
        return;

    uint32_t x_s = std::max(int32_t(0), (int32_t)(std::floor(x_min)));
    uint32_t x_e = std::min(int32_t(screen_width) - 1, (int32_t)(std::floor(x_max)));
    uint32_t y_s = std::max(int32_t(0), (int32_t)(std::floor(y_min)));
    uint32_t y_e = std::min(int32_t(screen_height) - 1, (int32_t)(std::floor(y_max)));
    for (size_t x = x_s; x <= x_e; x++)
    {
        for (size_t y = y_s; y <= y_e; y++)
        {
            point2D pixel = point2D(x, y);
            if(tri.contains(pixel))
            {
                surface[y*screen_width + x] = '#';
            }
        }
    }
}

void draw_segment(const geometry2D::segment<double>& segment, std::vector<char>& surface)
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
            surface[y*screen_width + x] = '#';
        }
    }
    else
    {
        //clip segment to screen rect
        auto intersect = geometry2D::intersection(geometry2D::rect<double>(0, 0, screen_width, screen_height), segment);

        if(intersect)
        {
            auto& result = *intersect;
            if(result.index() == 1)
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

#include <iostream>

int main()
{
    using namespace geometry;
    using namespace geometry::geometry_io;

    sphere<double, 3> circle(
        point_type<double, 3>(-3, 4, 5),
        point_type<double, 3>(6, 2, 2),
        point_type<double, 3>(15, 3, 9),
        point_type<double, 3>(8, 2, 1)
    );

    std::cout << circle;
    std::cin.get();
    geometry::space<double, 1, 2> l1;
    l1[0] = { 1,3 };
    l1[1] = { 3,5 };
    geometry::space<double, 1, 2> l2;
    l2[0] = { 0,2 };
    l2[1] = { 2,4 };

    
    std::cout << l1 << std::endl;
    std::cout << l2 << std::endl;

    auto result = geometry::intersections::intersection(l1, l2);
    if(!result)
    {
        std::cout << "no intersection" << std::endl;
    }
    else
    {
        auto& r = *result;
        if(r.index() == 0)
        {
            std::cout << std::get<0>(r) << std::endl;
        }
        else if(r.index() == 1)
        {
            std::cout << std::get<1>(r) << std::endl;
        }
    }

    std::cin.get();

    auto console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
    SetConsoleActiveScreenBuffer(console);
    COORD size;
    size.X = screen_width;
    size.Y = screen_height;
    SetConsoleScreenBufferSize(console, size);
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    SetConsoleDisplayMode(console, CONSOLE_FULLSCREEN_MODE, nullptr);

    std::vector<char> screen(screen_width*screen_height);

    auto tp1 = std::chrono::high_resolution_clock::now();
    auto tp2 = std::chrono::high_resolution_clock::now();

    while(true)
    {
        tp2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> e_t = tp2 - tp1;
        tp1 = tp2;
        float elapsed_time = e_t.count();

        std::fill(screen.begin(), screen.end(), ' ');
       
        geometry3D_renderer::draw_cube(
            geometry3D_renderer::point3D(50, 50, 5.0),
            geometry3D_renderer::point3D(10, 10, 10),
            clock() / 1000.0,
            screen,
            screen_width,
            screen_height
        );

        //draw_segment(geometry2D::segment<double>(-100, -1, screen_width + 1, screen_height + 1), screen);

        DWORD dwBytesWritten = 0;
        std::stringstream ss;
        ss << 1.0f / elapsed_time;
        auto fps = ss.str();
        std::copy(fps.begin(), fps.end(), screen.begin());

        WriteConsoleOutputCharacter(console, screen.data(), screen_width * screen_height, { 0,0 }, &dwBytesWritten);
    }

    /*
    geometry2D::segment<double> a = geometry2D::segment<double>(point2D(0, 0), point2D(5, 0));
    geometry2D::segment<double> b = geometry2D::segment<double>(point2D(4, 0), point2D(7, 0));

    cout << "a=" << a << endl;
    cout << "b=" << b << endl;

    auto result = geometry2D::intersection(a, b);
    if(result)
    {
        if((*result).index() == 0)
        {
            cout << "intersection: " << std::get<0>(*result) << endl;
        }
        else if((*result).index() == 1)
        {
            cout << "intersection: " << std::get<1>(*result) << endl;
        }
    }
    else
    {
        cout << "no intersection" << endl;
    }*/
    return 0;
}