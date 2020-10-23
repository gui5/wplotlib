#pragma once
namespace wpl {

using scale_t = point_t<float>;

class axis_scale_t {
public:
    axis_scale_t(const scale_t& x, const scale_t& y) :
        _x(x), _y(y) {}

    axis_scale_t() = default;
    ~axis_scale_t() = default;

    axis_scale_t& set_x_scale(const scale_t& value) noexcept {
        _x = value;
        return *this;
    }

    axis_scale_t& set_x_scale(scale_t&& value) noexcept {
        _x = value;
        return *this;
    }

    axis_scale_t& set_x_scale(float x, float y)noexcept {
        _x = scale_t(x, y);
        return *this;
    }

    axis_scale_t& set_y_scale(const scale_t& value)noexcept {
        _y = value;
        return *this;
    }

    axis_scale_t& set_y_scale(scale_t&& value)noexcept {
        _y = value;
        return *this;
    }

    axis_scale_t& set_y_scale(float x, float y)noexcept {
        _y = scale_t(x, y);
        return *this;
    }

    const scale_t& y()const noexcept {
        return _y;
    }

    const scale_t& x()const noexcept {
        return _x;
    }

private:
    scale_t _x;
    scale_t _y;
};
}