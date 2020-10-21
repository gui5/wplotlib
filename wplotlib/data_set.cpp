#include "data_set.h"

using namespace wpl;

wpl::data_set_t::data_set_t(int buffer_limit) :_buffer_limit(buffer_limit),
_bad_sample(NAN, NAN) {}

const sample_t& wpl::data_set_t::operator[](int i) noexcept
{
    if (i < _buffer.size()) {
        return _buffer[i];
    }
    return _bad_sample;
}

const sample_t& wpl::data_set_t::at(int i) noexcept
{
    if (i < _buffer.size()) {
        return _buffer.at(i);
    }
    return _bad_sample;
}

void wpl::data_set_t::push(const sample_t& sample) noexcept
{
    static uint64_t idx = 0;
    if (idx >= _buffer_limit) {
        idx = 0;
    }
    _buffer[idx++] = sample;
}

void wpl::data_set_t::push(float value, float time) noexcept
{
    push(sample_t(value, time));
}

const size_t wpl::data_set_t::size() noexcept
{
    return size_t();
}

auto  wpl::data_set_t::begin() noexcept
{
    return _buffer.begin();
}

auto  wpl::data_set_t::end() noexcept
{
    return _buffer.end();
}

auto wpl::data_set_t::begin() const noexcept
{
    return _buffer.begin();
}

auto wpl::data_set_t::end() const noexcept
{
    return _buffer.end();
}
