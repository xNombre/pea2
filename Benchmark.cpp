#include "Benchmark.hpp"

#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "DynamicTSP.hpp"
#include "BranchnBoundTSP.hpp"
#include "TimeBench.hpp"
#include "RandomGraphGen.hpp"
#include "BruteForceTSP.hpp"

typedef std::unordered_map<std::type_index, std::pair<duration_t, size_t>> results_t; // duration, timeouts
typedef std::vector<std::pair<size_t, results_t>> results_array_t; // size, duration, timeouts

namespace Constants
{
    using namespace std::chrono_literals;

    static const size_t random_intances_per_size = 100;
    static const std::vector<size_t> sizes_to_test = { 3, 5, 7, 9, 10, 11, 12, 13 };
    
    static const duration_t timeout_time = 5min;
    static const size_t max_city_weight = 1000;
}

std::optional<duration_t> Benchmark::solve_tsp(std::shared_ptr<TSPAlgorithm> alg)
{
    using namespace std::chrono_literals;
    TimeBench<TSPResult> benchmark([&] { return alg->solve(); });
    auto result_fut = benchmark.start_benchmark(Constants::timeout_time);
    auto result = result_fut.get();

    if (result.task_finished) {
        return result.measured_time;
    }

    return std::nullopt;
}

std::string Benchmark::get_name_of_algorithm(std::type_index alg)
{
    if (alg == typeid(BruteForceTSP)) {
        return "Brute Force";
    }
    else if (alg == typeid(DynamicTSP)) {
        return "Dynamic";
    }
    else if (alg == typeid(BranchnBoundTSP<std::queue>)) {
        return "BnB queue";
    }
    else if (alg == typeid(BranchnBoundTSP<std::stack>)) {
        return "BnB stack";
    }
    else if (alg == typeid(BranchnBoundTSP<std::priority_queue>)) {
        return "BnB priority_queue";
    }

    return "";
}

void Benchmark::start_benchmark()
{
    results_array_t benchmark_results;

    for (const auto &size : Constants::sizes_to_test) {
        results_t averaged_results;

        for (size_t i = 0; i < Constants::random_intances_per_size; i++) {
            const auto &matrix = RandomGraphGen::generate(size, Constants::max_city_weight);

            std::unordered_map<std::type_index, std::shared_ptr<TSPAlgorithm>> algorithms = {
                {typeid(BruteForceTSP), std::make_shared<BruteForceTSP>(matrix)},
                {typeid(DynamicTSP), std::make_shared<DynamicTSP>(matrix)},
                {typeid(BranchnBoundTSP<std::queue>), std::make_shared<BranchnBoundTSP<std::queue>>(matrix)},
                {typeid(BranchnBoundTSP<std::stack>), std::make_shared<BranchnBoundTSP<std::stack>>(matrix)},
                {typeid(BranchnBoundTSP<std::priority_queue>), std::make_shared<BranchnBoundTSP<std::priority_queue>>(matrix)}
            };

            for (auto &alg : algorithms) {
                std::type_index type = typeid(BranchnBoundTSP<std::queue>);
                if (alg.first == type && size > 11)
                    continue;
                
                std::cout << "Running " << get_name_of_algorithm(alg.first) << " with size " << size << " iteration " << i << "\n";
                auto result = solve_tsp(alg.second);

                if (result.has_value()) {
                    averaged_results[alg.first].first += *result;
                    std::cout << "   done" << std::endl;
                }
                else {
                    averaged_results[alg.first].second++;
                    std::cout << "   timeout" << std::endl;
                }
            }
        }
        for (auto &result : averaged_results) {
            result.second.first /= Constants::random_intances_per_size - result.second.second;
        }

        benchmark_results.push_back(std::make_pair(size, std::move(averaged_results)));
    }

    for (auto &result : benchmark_results) {
        std::cout << "Results for size " << result.first << "\n";
        for (auto &result : result.second) {
            if (result.second.second == Constants::random_intances_per_size) {
                std::cout << "   Algorithm " << get_name_of_algorithm(result.first) << "timed out for all instances\n";
            }
            else {
                std::cout << "   Algorithm " << get_name_of_algorithm(result.first) << " average " << std::chrono::duration_cast<std::chrono::nanoseconds>(result.second.first).count() << "ns timeouts " << result.second.second << "\n";
            }
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}