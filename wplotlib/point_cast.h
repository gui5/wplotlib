#pragma once
#include "point.h"
namespace wpl {
template <typename T, typename K>
T point_cast(const point_t<K>& p)noexcept {
    static_assert(std::is_arithmetic_v < T>
        "Only arithmetic types are allowed");
    return T(p.x, p.y);
}
}