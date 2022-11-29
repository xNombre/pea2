#include "RandomGraphGen.hpp"

size_t RandomGraphGen::randomNumberWithinRange(const size_t &first, const size_t &second)
{
    std::uniform_int_distribution<size_t> distribution(first, second);

    return distribution(generator);
}

CitiesMatrix RandomGraphGen::generate(const size_t &cities, const size_t &max_weight)
{
    CitiesMatrix matrix(cities);

    for (size_t i = 0; i < cities; i++) {
        for (size_t j = 0; j < cities; j++) {
            if (i == j)
                continue;
            
            matrix.connect_cities_bidirectional(i, j, randomNumberWithinRange(10, max_weight));
        }
    }

    return matrix;
}