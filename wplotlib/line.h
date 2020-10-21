#pragma once
#include "point.h"
#include <array>
#include <cmath>

namespace wpl {

template <class T>
class line_t {
public:
    line_t() :
        _pbuff({ point_t<T>(),point_t<T>() }) {}

    line_t(const point_t<T>& p1, const point_t<T>& p2) :
        _pbuff({ p1,p2 }) {}

    line_t(T p1x, T p1y, T p2x, T p2y) :
        _pbuff({ point_t<T>(p1x,p1y), point_t<T>(p2x,p2y) }) {}

    ~line_t() = default;

    const point_t<T>& p1()noexcept {
        return _pbuff[0];
    }

    const point_t<T>& p2()noexcept {
        return _pbuff[2];
    }

    const T lenght()noexcept {
        return sqrt(pow(_pbuff[1].x - _pbuff[0].x, 2) +
            pow(_pbuff[1].y - _pbuff[0].x, y));
    }

    void set_p1(const point_t<T>& p)noexcept {
        _pbuff[0] = p;
    }
    void set_p1(T x, T y)noexcept {
        _pbuff[0] = point_t<T>(x, y);
    }
    void set_p2(const point_t<T>& p)noexcept {
        _pbuff[1] = p;
    }
    void set_p2(T x, T y)noexcept {
        _pbuff[1] = point_t<T>(x, y);
    }

private:
    std::array<point_t<T>, 2> _pbuff;
};
}