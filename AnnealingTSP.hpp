#pragma once

#include "TSPAlgorithm.hpp"

#include <random>

class AnnealingTSP : public TSPAlgorithm {
public:
    AnnealingTSP(const CitiesMatrix &matrix)
        : TSPAlgorithm(matrix),
        generator(random_dev()),
        random_probability(0.0, 1.0),
        cities_probability(0, matrix.get_cities_number() - 1),
        best_result(matrix.get_cities_number())
    {
        for (size_t i = 0; i < matrix.get_cities_number(); i++) {
            best_result.path[i] = i;
        }
    }
    
    TSPResult solve() override;

private:
    std::random_device random_dev;
    std::mt19937 generator;
    std::uniform_real_distribution<double> random_probability;
    std::uniform_int_distribution<size_t> cities_probability;
    
    TSPResult best_result;
    double temperature;
    double cooling_factor = 0.999997;

    double get_probability(const size_t cur_cost, const size_t next_cost) const;
    size_t get_cost(const TSPResult::path_arr_t &path) const;
    void generate_random_path();
    void cool_down();
    void set_initial_temp();
    TSPResult swap_random_cities(const TSPResult& cur_result);
};