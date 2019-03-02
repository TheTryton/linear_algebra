#pragma once

#include "primitives/line.hpp"
#include "primitives/segment.hpp"
#include "primitives/rect.hpp"
#include "primitives/triangle.hpp"

NAMESPACE_GEOMETRY2D_BEGIN

template<class T>
std::ostream& operator<<(std::ostream& os, const line<T>& l)
{
    os << "{line attachment_pt=" << l.p1() << " creating_vector=" << l.p2() - l.p1() << "}";
    return os;
}


template<class T>
std::ostream& operator<<(std::ostream& os, const segment<T>& s)
{
    os << "{segment p1=" << s.p1() << ", p2=" << s.p2() << "}";
    return os;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const rect<T>& r)
{
    os << "{rect x=" << r.x() << ", y=" << r.y() << ", w=" << r.width() << ", h=" << r.height() << "}";
    return os;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const triangle<T>& t)
{
    os << "{rect p1=" << t.p1() << ", p2=" << t.p2() << ", p3=" << t.p3() << "}";
    return os;
}

NAMESPACE_GEOMETRY2D_END