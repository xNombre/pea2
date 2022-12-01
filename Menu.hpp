#pragma once

#include <iostream>
#include <memory>

#include "CitiesGraphReader.hpp"
#include "CitiesMatrixPrinter.hpp"
#include "AnnealingTSP.hpp"
#include "TabuSearchTSP.hpp"
#include "ArrayPrinter.hpp"
#include "RandomGraphGen.hpp"
#include "TimeBench.hpp"
#include "TimeBench.cpp"
#include "Benchmark.hpp"

using namespace std;

long long getDataFromUser()
{
    long long val;
    //cout << "Podaj wartosc:\n";
    cin >> val;

    // Clear any garbage
    cin.clear();
    cin.ignore(10000, '\n');

    return val;
}

char getOptionFromUser()
{
    char input;
    std::cin >> input;

    // Clear any garbage
    cin.clear();
    cin.ignore(10000, '\n');

    return input;
}

void print_result(const TSPResult& result)
{
    std::cout << "Calkowity koszt: " << result.total_weight << "\n0 ";
    ArrayPrinter::print(result.path, false);
    std::cout << "0" << std::endl;
}

void solve_tsp(std::unique_ptr<TSPAlgorithm> alg)
{
    using namespace std::chrono_literals;
    TimeBench<TSPResult> benchmark([&] { return alg->solve(); });
    auto result_fut = benchmark.start_benchmark(2min);
    auto result = result_fut.get();
    if (result.task_finished) {
        print_result(result.result);
        std::cout << "Finished in " <<
            std::chrono::duration_cast<std::chrono::milliseconds>(result.measured_time).count()
            << "ms" << std::endl;
    }
    else {
        std::cout << "Timeout" << std::endl;
    }
}

void menu()
{
    CitiesMatrix graph;
    while (1) {
        char input;
        cout << "PEA Projekt 1.\n"
            << "f - odczyt z pliku\n"
            << "r - losowa generacja\n"
            << "x - wyswietl graf\n"
            << "a - brute-force\n"
            << "t - dynamic\n"
            << "s - benchmark\n"
            << "q - wyjscie\n";
        input = getOptionFromUser();

        switch (input) {
        case 'r': {
            auto size = getDataFromUser();
            graph = RandomGraphGen::generate(size);
            break;
        }
        case 'f': {
            string file;
            cin >> file;
            graph = CitiesGraphReader::readFromFile(file);
            break;
        }
        case 'x': {
            CitiesMatrixPrinter::print(graph);
            break;
        }
        case 'a': {
            solve_tsp(std::make_unique<TabuSearchTSP>(graph));
            break;
        }
        case 't': {
            solve_tsp(std::make_unique<AnnealingTSP>(graph));
            break;
        }
        case 'q': {
            return;
        }
        case 's': {
            Benchmark bench;
            bench.start_benchmark();
            break;
        }
        }
    }
}