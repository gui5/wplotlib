#pragma once
#include "point.h"
#include "scale.h"
#include <vector>

namespace wpl {
class grid_t;

struct label_t
{
    point_t<int> position;
    float value;

    label_t(const point_t<int>& position, float value) :
        position(position), value(value) {}
    label_t() :
        value(0.0) {}

    ~label_t() = default;

    bool operator==(const label_t &obj)const noexcept {
        return value == obj.value;
    }

    bool operator<(const label_t &obj)const noexcept {
        return value < obj.value;
    }

    bool operator>(const label_t &obj)const noexcept {
        return value > obj.value;
    }

    bool operator<=(const label_t &obj)const noexcept {
        return value <= obj.value;
    }

    bool operator>=(const label_t &obj)const noexcept {
        return value >= obj.value;
    }
};