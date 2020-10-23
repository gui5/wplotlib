#pragma once
#include "point.h"
#include <array>
#include <cmath>

namespace wpl {

template <class T>
class line_t {
    static_assert(std::is_arithmetic_v<T>,
        "Only arithmetic types are allowed");
public:
    line_t() :
        _p1(point_t<T>()), _p2(point_t<T>) {}

    line_t(const point_t<T>& p1, const point_t<T>& p2) :
        _p1(p1), _p2(p2) {}

    line_t(point_t<T>&& p1, point_t<T>&& p2) :
        _p1(std::move(p1)), _p2(std::move(p2)) {}

    line_t(const T& p1x, const  T& p1y, const  T& p2x, const T& p2y) :
        _p1(point_t<T>(p1x, p1y)), _p2(point_t<T>(p2x, p2y)) {}

    line_t(T&& p1x, T&& p1y, T&& p2x, T&& p2y) :
        _p1(point_t<T>(p1x, p1y)), _p2(point_t<T>(p2x, p2y)) {}

    ~line_t() = default;

    const point_t<T>& p1()const noexcept {
        return _p1;
    }

    const point_t<T>& p2()const noexcept {
        return _p2;
    }

    const T lenght() const noexcept {
        return sqrt(pow(_p2.x - _p1.x, 2) +
            pow(_p2.y - _p1.x, y));
    }

    line_t<point_t<T>>& set_p1(const point_t<T>& p)noexcept {
        _p1 = p;
        return *this;
    }

    line_t<point_t<T>>& set_p1(const T& x, const T& y)noexcept {
        _p1 = point_t<T>(x, y);
        return *this;
    }

    line_t<point_t<T>>& set_p1(point_t<T>&& p)noexcept {
        _p1 = p;
        return *this;
    }

    line_t<point_t<T>>& set_p1(T&& x, T&& y)noexcept {
        _p1 = point_t<T>(x, y);
        return *this;
    }

    line_t<point_t<T>>& set_p2(const point_t<T>& p)noexcept {
        _p2 = p;
        return *this;
    }

    line_t<point_t<T>>& set_p2(const T& x, const T& y)noexcept {
        _p2 = point_t<T>(x, y);
        return *this;
    }

    bool operator==(const line_t<T>& obj)const noexcept {
        return lenght() == obj.lenght();
    }

    bool operator<(const line_t<T>& obj)const noexcept {
        return lenght() < obj.lenght();
    }

    bool operator>(const line_t<T>& obj)const noexcept {
        return lenght() > obj.lenght();
    }

    bool operator<=(const line_t<T>& obj)const noexcept {
        return lenght() <= obj.lenght();
    }

    bool operator>=(const line_t<T>& obj)const noexcept {
        return lenght() >= obj.lenght();
    }


private:
    point_t<T> _p1;
    point_t<T> _p2;
};
}