#pragma once
#include <type_traits>

namespace wpl {
template <class T>
struct point_t {
    static_assert(std::is_arithmetic_v<T>,
        "Only arithmetic types are allowed");

    T x;
    T y;

    point_t() :
        x(static_cast<T>(0)), y(static_cast<T>(0)) {}

    point_t(T x, T y) :
        x(x), y(y) {}

    ~point_t() = default;
};
}