#pragma once
#include "point.h"
#include <memory>
#include <queue>

namespace wpl {

/*
    X = time, Y = value
*/
template<typename T>
using sample_t = point_t<T>;

template<typename T>
class data_set_t {
    static_assert(std::is_arithmetic_v<T>,
        "Only arithmetic types are allowed");

public:
    data_set_t(uint64_t buffer_limit = 1024) :
        _buffer_size(buffer_limit), _idx(0) {

        if constexpr (std::is_floating_point_v<T>) {
            _bad_sample = sample_t<T>(NAN, NAN);
        }
        else {
            _bad_sample = sample_t<T>(0, 0);
        }

        _buffer = std::make_unique<sample_t<T>>(_buffer_size);
    }

    ~data_set_t() = default;

    const sample_t<T>& operator[](uint64_t i)const noexcept {
        if (i < _buffer_size) {
            return _buffer.get()[i];
        }
        return _bad_sample;
    }

    sample_t<T>& operator[](uint64_t i) noexcept {
        if (i < _buffer_size) {
            return _buffer.get()[i];
        }
        return _bad_sample;
    }

    const sample_t<T>& at(int i)const noexcept {
        if (i < _buffer_size) {
            return _buffer.get()[i];
        }
        return _bad_sample;
    }

    data_set_t& push(const sample_t<T>& sample)noexcept {
        if (_idx >= _buffer_size) {
            _idx = 0;
        }
        _buffer.get()[_idx++] = sample;

    }

    data_set_t& push(sample_t<T>&& sample)noexcept {
        if (_idx >= _buffer_size) {
            _idx = 0;
        }
        _buffer.get()[_idx++] = sample;

    }

    data_set_t& push(const T& value, const T& time)noexcept {
        if (_idx >= _buffer_size) {
            _idx = 0;
        }
        _buffer.get()[_idx++] = sample_t<T>(time, value);
    }

    data_set_t& push(T&& value, T&& time)noexcept {
        if (_idx >= _buffer_size) {
            _idx = 0;
        }
        _buffer.get()[_idx++] = sample_t<T>(time, value);
    }

    const size_t size() const noexcept {
        _buffer_size;
    }

    data_set_t& resize(uint64_t size)noexcept {
        _buffer_size = size;
        _buffer = std::make_unique<sample_t<T>>(_buffer_size);
    }

    const float time_at(uint64_t i)const noexcept {
        if (i < _buffer_size) {
            return _buffer.get()[i].x;
        }

        if constexpr (std::is_floating_point_v<T>) {
            return NAN;
        }
        else {
            return 0;
        }
    }

    const float value_at(uint64_t i)const noexcept {
        if (i < _buffer_size) {
            return _buffer.get()[i].y;
        }

        if constexpr (std::is_floating_point_v<T>) {
            return NAN;
        }
        else {
            return 0;
        }
    }

    bool operator==(const data_set_t<T>& obj)const noexcept {
        return (_buffer_size == obj._buffer_size &&
            (memcmp(_buffer.get(), obj._buffer.get(), _buffer_size) == 0));
    }

    bool operator<(const data_set_t<T>& obj)const noexcept {
        return _buffer_size < obj._buffer_size;
    }

    bool operator>(const data_set_t<T>& obj)const noexcept {
        return _buffer_size > obj._buffer_size;
    }

    bool operator<=(const data_set_t<T>& obj)const noexcept {
        return  (_buffer_size < obj._buffer_size) || ((_buffer_size == obj._buffer_size &&
            (memcmp(_buffer.get(), obj._buffer.get(), _buffer_size) == 0)));
    }

    bool operator>=(const data_set_t<T>& obj)const noexcept {
        return  (_buffer_size > obj._buffer_size) || ((_buffer_size == obj._buffer_size &&
            (memcmp(_buffer.get(), obj._buffer.get(), _buffer_size) == 0)));
    }




private:
    uint64_t _buffer_size;
    std::unique_ptr<sample_t<T>> _buffer;
    sample_t<T> _bad_sample;
    uint64_t _idx;
};
}