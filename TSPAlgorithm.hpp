#pragma once

#include "Array.hpp"
#include "CitiesMatrix.hpp"

struct TSPResult {
    typedef Array<size_t> path_arr_t;
    size_t total_weight;
    path_arr_t path;

    TSPResult() { }
    TSPResult(const size_t size)
        : path(size)
    {}
};

class TSPAlgorithm {
public:
    TSPAlgorithm(const CitiesMatrix &matrix)
        : matrix(matrix)
    {
        if (!matrix.is_matrix_valid()) {
            throw std::invalid_argument("Matrix is not valid!");
        }
    }

    virtual TSPResult solve() = 0;
    virtual ~TSPAlgorithm() = default;

protected:
    CitiesMatrix const& matrix;
};