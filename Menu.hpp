#pragma once

#include <iostream>
#include <memory>

#include "CitiesGraphReader.hpp"
#include "CitiesMatrixPrinter.hpp"
#include "BruteForceTSP.hpp"
#include "ArrayPrinter.hpp"
#include "BranchnBoundTSP.hpp"
#include "RandomGraphGen.hpp"
#include "DynamicTSP.hpp"
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

std::unique_ptr<TSPAlgorithm> choose_bnb_container(const CitiesMatrix &graph)
{
    std::unique_ptr<TSPAlgorithm> ptr;

    char input;
    cout << "Wybierz kontener:\n"
        << "p - priority_queue\n"
        << "s - stack\n"
        << "q - queue\n";
    input = getOptionFromUser();

    switch (input) {
    case 'p': {
        ptr = std::make_unique<BranchnBoundTSP<std::priority_queue>>(graph);
        break;
    }
    case 's': {
        ptr = std::make_unique<BranchnBoundTSP<std::stack>>(graph);
        break;
    }
    case 'q': {
        ptr = std::make_unique<BranchnBoundTSP<std::queue>>(graph);
        break;
    }
    }

    return ptr;
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
            << "b - brute-force\n"
            << "a - branch&bound\n"
            << "d - dynamic\n"
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
        case 'b': {
            solve_tsp(std::make_unique<BruteForceTSP>(graph));
            break;
        }
        case 'a': {
            solve_tsp(choose_bnb_container(graph));
            break;
        }
        case 'd': {
            solve_tsp(std::make_unique<DynamicTSP>(graph));
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