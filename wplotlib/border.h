#pragma once
#include "point.h"
#include <array>

namespace wpl {
class border_t {

    enum edge {
        left,
        right,
        up,
        down
    };

public:

    border_t(int left, int right, int up, int down) :
        _borders({ left,right,up,down }) {}

    border_t() :
        _borders({ 0,0,0,0 }) {}

    ~border_t() = default;

    void set_edge(const edge& edg, const int value)noexcept {
        _borders[edg] = value;
    }

    int get_edge(const edge& edg) {
        return _borders[edg];
    }

    int x_length(int)noexcept {
        return sqrt(pow(_borders[edge::right] - _borders[edge::left], 2));
    }

    int y_length()noexcept {
        return sqrt(pow(_borders[edge::down] - _borders[edge::up], 2));
    }
    const point_t<int> position()noexcept {
        return point_t<int>(_borders[edge::left], _borders[edge::up]);
    }

private:
    std::array<int, 4> _borders;
};

}