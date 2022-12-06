#include "AnnealingTSP.hpp"

#include <math.h>

TSPResult AnnealingTSP::solve()
{
    set_initial_temp_and_path();
    std::chrono::time_point end_time = std::chrono::system_clock::now() + timeout;

    TSPResult cur_result = best_result;
    do {
        TSPResult next_result = swap_random_cities(cur_result);

        if (next_result.total_weight < best_result.total_weight) {
            best_result = next_result;
        }
        
        if (cur_result.total_weight > next_result.total_weight) {
            cur_result = std::move(next_result);
        }
        else if (random_probability(generator) <
            get_probability(cur_result.total_weight, next_result.total_weight)) {
            cur_result = std::move(next_result);
        }

        cool_down();
    } while (temperature > 1 && (end_time - std::chrono::system_clock::now()).count() > 0);

    return best_result;
}

double AnnealingTSP::get_probability(const size_t cur_cost, const size_t next_cost) const
{
    return std::exp(-(double)(next_cost - cur_cost) / temperature);
}

size_t AnnealingTSP::get_cost(const TSPResult::path_arr_t &path) const
{
    size_t last = path[0];
    size_t sum = matrix.at(0, last);

    for (size_t city = 1; city < path.size(); city++) {
        sum += matrix.at(last, path[city]);
        last = path[city];
    }
    sum += matrix.at(last, 0);

    return sum;
}

void AnnealingTSP::generate_random_path()
{
    // Swap two random cities 2*cities_number times
    const auto iterations = matrix.get_cities_number() * 2;

    for (size_t i = 0; i < iterations; i++) {
        std::swap(best_result.path[cities_probability(generator)],
                  best_result.path[cities_probability(generator)]);
    }
    best_result.total_weight = get_cost(best_result.path);
}

void AnnealingTSP::cool_down()
{
    (*cool_fn)(temperature, cooling_factor);
}

void AnnealingTSP::set_initial_temp_and_path()
{
    const size_t iterations = 5;
    
    for (auto i = 0; i < iterations; i++) {
        generate_random_path();
        temperature += best_result.total_weight;
    }

    temperature /= iterations;
}

TSPResult AnnealingTSP::swap_random_cities(const TSPResult &cur_result)
{
    TSPResult result = cur_result;
    size_t a = cities_probability(generator), b, zero = 0;

    do {
        b = cities_probability(generator);
    } while (a == b);

    bool nearby = b - a == 1 || a - b == 1;
    if (nearby) {
        const auto& min = std::min(a, b);
        const auto& max = std::max(a, b);
        
        auto &before_swp = (min == 0 ? zero : result.path[min - 1]);
        auto &a_swapee = result.path[min];
        auto &b_swapee = result.path[max];
        auto &after_swp = (max == result.path.size() - 1 ? zero : result.path[max + 1]);

        const auto old_weight = matrix.at(before_swp, a_swapee) +
            matrix.at(a_swapee, b_swapee) +
            matrix.at(b_swapee, after_swp);
        const auto new_weight = matrix.at(before_swp, b_swapee) +
            matrix.at(b_swapee, a_swapee) +
            matrix.at(a_swapee, after_swp);
        result.total_weight += new_weight;
        result.total_weight -= old_weight;

        std::swap(a_swapee, b_swapee);
    }
    else {
        auto &before_a = (a == 0 ? zero : result.path[a - 1]);
        auto &a_swapee = result.path[a];
        auto &after_a = (a == result.path.size() - 1 ? zero : result.path[a + 1]);

        auto &before_b = (b == 0 ? zero : result.path[b - 1]);
        auto &b_swapee = result.path[b];
        auto &after_b = (b == result.path.size() - 1 ? zero : result.path[b + 1]);

        const auto old_weight = matrix.at(before_a, a_swapee) +
            matrix.at(a_swapee, after_a) +
            matrix.at(before_b, b_swapee) +
            matrix.at(b_swapee, after_b);
        const auto new_weight = matrix.at(before_a, b_swapee) +
            matrix.at(b_swapee, after_a) +
            matrix.at(before_b, a_swapee) +
            matrix.at(a_swapee, after_b);
        result.total_weight += new_weight;
        result.total_weight -= old_weight;
        
        std::swap(a_swapee, b_swapee);
    }
    
    return result;
}