#pragma once
namespace wpl {

struct scale_t
{
    float max;
    float min;

    scale_t(float max = 10.0, float min = -10.0) :
        max(max), min(min) {}

    ~scale_t() = default;
};

class axis_scale_t {
public:
    axis_scale_t(const scale_t& x, const scale_t& y) :
        _x(x), _y(y) {}

    axis_scale_t() = default;
    ~axis_scale_t() = default;

    void set_x_scale(const scale_t & value)noexcept {
        _x = value;
    }

    void set_x_scale(float x, float y)noexcept {
        _x = scale_t(x, y);
    }

    void set_y_scale(const scale_t& value)noexcept {
        _y = value;
    }

    void set_y_scale(float x, float y)noexcept {
        _y = scale_t(x, y);
    }

    const scale_t& x()noexcept {
        return _x;
    }

    const scale_t& y()noexcept {
        return _y;
    }


private:
    scale_t _x;
    scale_t _y;
};
}