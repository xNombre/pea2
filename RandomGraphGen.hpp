#pragma once

#include <random>

#include "CitiesMatrix.hpp"

static std::random_device rd;
static std::default_random_engine generator(rd());

class RandomGraphGen
{
public:
    static CitiesMatrix generate(const size_t &cities, const size_t &max_weight = 100);

private:
    static size_t randomNumberWithinRange(const size_t &first, const size_t &second);
};