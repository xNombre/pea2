#include "CitiesMatrixPrinter.hpp"

#include <iostream>

using namespace std;

#define RST  "\x1B[0m"
#define KRED  "\x1B[41m"
#define KBLU  "\x1B[44m"
#define KMAG  "\x1B[35m"

void CitiesMatrixPrinter::print(const CitiesMatrix &graph)
{
    const auto &count = graph.get_cities_number();
    const auto &vertices = graph.get_cities_number();

    if (vertices == 0) {
        std::cout << KRED << "Graf jest pusty!" << RST << std::endl;
        return;
    }

    // Column headline
    std::cout << "   " << KRED;
    for (size_t j = 0; j < count; j++) {
        std::cout << j << "  ";

        if (j < 10)
            std::cout << " ";
    }
    std::cout << RST << std::endl;

    for (size_t i = 0; i < vertices; i++) {
        // Row headline
        std::cout << KBLU << i << RST << " ";
        if (i < 10) {
            std::cout << " ";
        }

        // Values
        for (size_t j = 0; j < count; j++) {
            // Show unreachable as -1
            if (graph.at(i, j) == CitiesMatrix::unreachable_val) {
                std::cout << KMAG << "INF " << RST;
                continue;
            }

            std::cout << graph.at(i, j) << " ";
            if (graph.at(i, j) < 10)
                std::cout << "  ";
            else if (graph.at(i, j) >= 10)
                std::cout << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}