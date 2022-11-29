#pragma once

#include <chrono>
#include <memory>
#include <optional>
#include <typeindex>
#include <string>

#include "TSPAlgorithm.hpp"

typedef std::chrono::duration<double> duration_t;

class Benchmark {
public:
    void start_benchmark();

private:
    std::optional<duration_t> solve_tsp(std::shared_ptr<TSPAlgorithm> alg);
    std::string get_name_of_algorithm(std::type_index alg);
};