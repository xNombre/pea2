#include "AnnealingTSP.hpp"

#include <math.h>

TSPResult AnnealingTSP::solve()
{
    generate_random_path();
    set_initial_temp();

    //size_t iterations = matrix.get_cities_number() * 10'000;

    TSPResult cur_result = best_result;
    do {
        TSPResult next_result = swap_random_cities(cur_result);
        
        if (random_probability(generator) <
            get_probability(cur_result.total_weight, next_result.total_weight)) {
            cur_result = std::move(next_result);
            if (cur_result.total_weight < best_result.total_weight) {
                best_result = cur_result;
            }
        }

        //iterations--;
        cool_down();
    } while (temperature > 1);

    return best_result;
}

double AnnealingTSP::get_probability(const size_t cur_cost, const size_t next_cost) const
{
    return std::min(std::exp(-(next_cost - cur_cost) / temperature), 1.0);
}

size_t AnnealingTSP::get_cost(const TSPResult::path_arr_t &path) const
{
    size_t last = path[0];
    size_t sum = matrix.at(0, last);

    for (size_t city = 1; city < matrix.get_cities_number() - 1; city++) {
        sum += matrix.at(last, path[city]);
    }

    sum += matrix.at(last, 0);

    return sum;
}

void AnnealingTSP::generate_random_path()
{
    // Swap two random cities 2*cities_number times
    //TSPResult result;
    const auto iterations = matrix.get_cities_number() * 2;

    for (size_t i = 0; i < iterations; i++) {
        std::swap(best_result.path[cities_probability(generator)],
                  best_result.path[cities_probability(generator)]);
    }
    best_result.total_weight = get_cost(best_result.path);

    //return result;
}

void AnnealingTSP::cool_down()
{
    temperature *= cooling_factor;
}

void AnnealingTSP::set_initial_temp()
{
    temperature = best_result.total_weight * matrix.get_cities_number();
}

TSPResult AnnealingTSP::swap_random_cities(const TSPResult &cur_result)
{
    TSPResult result = cur_result;
    size_t a = cities_probability(generator), b, zero = 0;

    do {
        b = cities_probability(generator);
    } while (a == b);

    auto &before_a = (a == 0 ? zero : result.path[a - 1]);
    auto &a_swapee = result.path[a];
    auto &after_a = (a == matrix.get_cities_number() ? zero :  result.path[a + 1]);

    auto &before_b = (b == 0 ? zero : result.path[b - 1]);
    auto &b_swapee = result.path[b];
    auto &after_b = (b == matrix.get_cities_number() ? zero : result.path[b + 1]);

    const auto old_weight = matrix.at(result.path[a - 1], result.path[a]) +
        matrix.at(result.path[a], result.path[a + 1]) +
        matrix.at(result.path[b - 1], result.path[b]) +
        matrix.at(result.path[b], result.path[b + 1]);
    const auto new_weight = matrix.at(result.path[a - 1], result.path[b]) +
        matrix.at(result.path[b], result.path[a + 1]) +
        matrix.at(result.path[b - 1], result.path[a]) +
        matrix.at(result.path[a], result.path[b + 1]);
    result.total_weight -= old_weight;
    result.total_weight += new_weight;
    
    std::swap(result.path[a], result.path[b]);

    return result;
}