#pragma once

#include "Array.hpp"
#include "CitiesMatrix.hpp"

struct TSPResult {
    size_t total_weight;
    Array<size_t> path;
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