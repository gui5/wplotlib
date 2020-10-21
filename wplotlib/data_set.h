#pragma once
#include "point.h"
#include <vector>

namespace wpl {

using sample_t = point_t<float>;

class data_set_t {
public:
    data_set_t(int buffer_limit = 1024);

    ~data_set_t() = default;

    const sample_t& operator[](int i)noexcept;
    const sample_t& at(int i)noexcept;

    void push(const sample_t& sample)noexcept;
    void push(float value, float time)noexcept;

    const size_t size()noexcept;

    auto begin()noexcept;
    auto end()noexcept;

    auto begin()const noexcept;
    auto end()const noexcept;

private:
    int _buffer_limit;
    std::vector<sample_t> _buffer;

    sample_t _bad_sample;
};
}