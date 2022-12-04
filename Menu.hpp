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
#include "GeometricCooling.hpp"
#include "LogaritmicCooling.hpp"
#include "LinearCooling.hpp"

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

void solve_tsp(std::unique_ptr<TSPAlgorithm> alg, std::chrono::duration<double> duration)
{
    TimeBench<TSPResult> benchmark([&] { return alg->solve(); });
    auto result_fut = benchmark.start_benchmark(duration);
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
    using namespace std::chrono_literals;
    std::shared_ptr<CoolingFunc> cooling_fn =
        std::static_pointer_cast<CoolingFunc>(std::make_shared<GeometricCooling>());
    double cooling_factor = 0.999997;
    CitiesMatrix graph;
    std::chrono::duration<double> timeout = 2min;
    while (1) {
        char input;
        cout << "PEA Projekt 1.\n"
            << "f - odczyt z pliku\n"
            << "r - losowa generacja\n"
            << "h - timeout\n"
            << "x - wyswietl graf\n"
            << "a - TabuSearch\n"
            << "t - SimulatedAnnealing\n"
            << "c - cooling factor\n"
            << "v - cooling function\n"
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
            solve_tsp(std::make_unique<TabuSearchTSP>(graph), timeout);
            break;
        }
        case 't': {
            solve_tsp(std::make_unique<AnnealingTSP>(graph, cooling_factor, cooling_fn), timeout);
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
        case 'h': {
            cout << "Podaj czas [s]:";
            timeout = std::chrono::seconds(getDataFromUser());
            break;
        }
        case 'v': {
            cout << "o - logarithmic\n"
                << "l - linear\n"
                << "g - geometric\n";
            
            auto c = getOptionFromUser();
            switch (c) {
            case 'o':
                cooling_fn =
                    std::static_pointer_cast<CoolingFunc>(std::make_shared<LogaritmicCooling>());
                break;
            case 'l':
                cooling_fn =
                    std::static_pointer_cast<CoolingFunc>(std::make_shared<LinearCooling>());
                break;
            case 'g':
                cooling_fn =
                    std::static_pointer_cast<CoolingFunc>(std::make_shared<GeometricCooling>());
                break;
            }
            
            break;
        }
        case 'c': {
            cout << "Podaj cooling factor:";
            scanf("%lf", &cooling_factor);
            break;
        }
        }
    }
}