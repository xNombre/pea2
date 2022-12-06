#pragma once

#include <memory>
#include <chrono>

#include "TSPAlgorithm.hpp"
#include "CoolingFunc.hpp"
#include "GeometricCooling.hpp"

#include <random>

class AnnealingTSP : public TSPAlgorithm {
public:
    AnnealingTSP(const CitiesMatrix &matrix,
                 const double cooling_factor,
                 std::shared_ptr<CoolingFunc> cool_fn,
                 std::chrono::duration<double> timeout)
        : TSPAlgorithm(matrix),
        cooling_factor(cooling_factor),
        cool_fn(cool_fn),
        generator(random_dev()),
        random_probability(0.0, 1.0),
        // Only allocate space for cities between first and last city (0)
        best_result(matrix.get_cities_number() - 1),
        // Range of city indexes in TSPResult
        cities_probability(0, matrix.get_cities_number() - 2),
        timeout(timeout)
    {
        for (size_t i = 0; i < matrix.get_cities_number() - 1; i++) {
            best_result.path[i] = i + 1;
        }
    }
    
    TSPResult solve() override;

private:
    std::chrono::duration<double> timeout;
        
    std::shared_ptr<CoolingFunc> cool_fn;
    
    std::random_device random_dev;
    std::mt19937 generator;
    std::uniform_real_distribution<double> random_probability;
    std::uniform_int_distribution<size_t> cities_probability;
    
    TSPResult best_result;
    
    double temperature;
    double cooling_factor;

    double get_probability(const size_t cur_cost, const size_t next_cost) const;
    size_t get_cost(const TSPResult::path_arr_t &path) const;
    size_t get_cost_parial(const TSPResult::path_arr_t &path, size_t from, size_t to) const;
    void generate_random_path();
    void cool_down();
    void set_initial_temp_and_path();
    TSPResult swap_random_cities(const TSPResult& cur_result);
};