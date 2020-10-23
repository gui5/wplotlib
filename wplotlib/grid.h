#pragma once
#include <vector>
#include "line.h"
namespace wpl {
class grid_t {
public:

    grid_t(uint32_t nrows = 0, uint32_t ncols = 0) :
        _nrows(nrows), _ncols(ncols) {}
    ~grid_t() = default;

    const int ncols()const noexcept {
        return _ncols;
    }

    const int nrows()const noexcept {
        return _nrows;
    }

    const std::vector<line_t<uint32_t>>& rows()const noexcept {
        return _rows;
    }

    const std::vector<line_t<uint32_t>>& cols()const noexcept {
        return _cols;
    }

    std::vector<line_t<uint32_t>>& rows() noexcept {
        return _rows;
    }

    std::vector<line_t<uint32_t>>& cols() noexcept {
        return _cols;
    }

    grid_t& set_rows(int value)noexcept {
        _nrows = value;
        return *this;
    }

    grid_t& set_cols(int value)noexcept {
        _ncols = value;
        return *this;
    }

private:
    uint32_t _ncols;
    uint32_t _nrows;

    std::vector<line_t<uint32_t>> _cols;
    std::vector<line_t<uint32_t>> _rows;
};
}