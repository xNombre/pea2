#pragma once

#include <fstream>

#include "CitiesMatrix.hpp"

class CitiesGraphReader {
public:
    static CitiesMatrix readFromFile(const std::string &filename, bool junk_in_file);
private:
    static std::ifstream openFile(const std::string &filename);
    static CitiesMatrix readCostsMatrix(std::ifstream &fileStream, const size_t &citiesCount);
};