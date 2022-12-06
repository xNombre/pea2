#include "Benchmark.hpp"

#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "TimeBench.hpp"
#include "CitiesGraphReader.hpp"
#include "AnnealingTSP.hpp"
#include "TabuSearchTSP.hpp"
#include "ArrayPrinter.hpp"

typedef std::unordered_map<std::type_index, TSPResult> results_t;
typedef std::vector<std::pair<std::string, results_t>> results_array_t; // size, duration, timeouts

namespace Constants
{
    using namespace std::chrono_literals;

    static const size_t random_intances_per_size = 10;
    static const double cooling_factor = 0.999999997;

    static const duration_t timeout_time = 5min;

    static const std::vector<std::string> filenames = { "ftv47.atsp",
        "ftv170.atsp" , "rbg403.atsp" };
    static const std::vector<std::chrono::duration<double>> timeouts = { 20s, 40s, 80s };
}

TSPResult Benchmark::solve_tsp(std::shared_ptr<TSPAlgorithm> alg)
{
    using namespace std::chrono_literals;
    TimeBench<TSPResult> benchmark([&] { return alg->solve(); });
    auto result_fut = benchmark.start_benchmark(Constants::timeout_time);
    auto result = result_fut.get();

    return result.result;
}

std::string Benchmark::get_name_of_algorithm(std::type_index alg)
{
    if (alg == typeid(AnnealingTSP)) {
        return "Annealing";
    }
    else if (alg == typeid(TabuSearchTSP)) {
        return "Tabu Search";
    }

    return "";
}

void Benchmark::start_benchmark()
{
    results_array_t benchmark_results;

    for (const auto &file : Constants::filenames) {
        for (const auto &timeout : Constants::timeouts) {
            results_t averaged_results;

            for (size_t i = 0; i < Constants::random_intances_per_size; i++) {
                const auto &matrix = CitiesGraphReader::readFromFile(file, true);

                std::unordered_map<std::type_index, std::shared_ptr<TSPAlgorithm>> algorithms = {
                    {typeid(AnnealingTSP), std::make_shared<AnnealingTSP>(matrix,
                    Constants::cooling_factor,
                    std::static_pointer_cast<CoolingFunc>(std::make_shared<GeometricCooling>()),
                    timeout)},
                };

                size_t minimum = SIZE_MAX;
                for (auto &alg : algorithms) {
                    std::cout << "Running " << get_name_of_algorithm(alg.first) << " with file " << file << " iteration " << i << "\n";
                    auto result = solve_tsp(alg.second);

                    if (result.total_weight < minimum)
                    {
                        minimum = result.total_weight;
                        averaged_results[alg.first] = result;
                    }

                    std::cout << "   done" << std::endl;
                }
            }

            benchmark_results.push_back(std::make_pair(file, std::move(averaged_results)));
        }
    }

    for (auto &result : benchmark_results) {
        std::cout << "Results for file " << result.first << "\n";
        for (auto &result : result.second) {
            std::cout << "   Algorithm " << get_name_of_algorithm(result.first) << " minimum weight " << result.second.total_weight << "\n Path: ";
            std::cout << "0 ";
            ArrayPrinter::print(result.second.path, false);
            std::cout << "0" << std::endl;
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}