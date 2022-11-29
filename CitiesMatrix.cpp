#include "CitiesMatrix.hpp"

#include <stdexcept>

const weight_t CitiesMatrix::unreachable_val = SIZE_MAX;

CitiesMatrix::CitiesMatrix(const size_t &cities_number)
    : cities_number(cities_number)
{
#ifdef DEBUG
    matrix = new weight_t[cities_number * cities_number]();
#else
    matrix = new weight_t[cities_number * cities_number];
#endif // DEBUG
    
    for (weight_t i = 0; i < cities_number; i++) {
        at(i, i) = unreachable_val;
    }
}

CitiesMatrix::~CitiesMatrix()
{
    delete[] matrix;
}

CitiesMatrix::CitiesMatrix(CitiesMatrix &&other)
{
    cities_number = other.cities_number;
    matrix = other.matrix;

    other.matrix = nullptr;
    other.cities_number = 0;
}

CitiesMatrix &CitiesMatrix::operator=(CitiesMatrix &&other)
{
    if (this == &other)
        return *this;

    delete[] matrix;
    
    cities_number = other.cities_number;
    matrix = other.matrix;

    other.matrix = nullptr;
    other.cities_number = 0;

    return *this;
}

CitiesMatrix::CitiesMatrix(const CitiesMatrix &other)
{
    cities_number = other.cities_number;

#ifdef DEBUG
    matrix = new weight_t[cities_number * cities_number]();
#else
    matrix = new weight_t[cities_number * cities_number];
#endif // DEBUG

    std::copy(other.matrix, other.matrix + cities_number * cities_number, matrix);
}

void CitiesMatrix::connect_cities(const size_t &from, const size_t &to, const size_t &weight)
{
#ifdef DEBUG
    if (from == to)
        throw std::invalid_argument("");
#endif // DEBUG

    at(from, to) = weight;
}

void CitiesMatrix::connect_cities_bidirectional(const size_t &from,
                                                const size_t &to,
                                                const size_t &weight)
{
    connect_cities(from, to, weight);
    connect_cities(to, from, weight);
}

bool CitiesMatrix::is_matrix_valid() const
{
    for (size_t i = 0; i < cities_number; i++) {
        for (size_t j = 0; j < cities_number; j++) {
            if (at(i, j) == 0)
                return false;
            if (at(i, j) == -1 && i != j)
                return false;
        }
    }

    return true;
}

const weight_t &CitiesMatrix::at(const size_t &from, const size_t &to) const
{
#ifdef DEBUG
    if (!matrix)
        throw std::runtime_error("Matix is null!");

    if (from >= cities_number || to >= cities_number)
        throw std::out_of_range("");
#endif // DEBUG

    return matrix[from * cities_number + to];
}

weight_t &CitiesMatrix::at(const size_t &from, const size_t &to)
{
#ifdef DEBUG
    if (!matrix)
        throw std::runtime_error("Matix is null!");
    
    if (from >= cities_number || to >= cities_number)
        throw std::out_of_range("");
#endif // DEBUG

    return matrix[from * cities_number + to];
}

const size_t &CitiesMatrix::get_cities_number() const
{
    return cities_number;
}