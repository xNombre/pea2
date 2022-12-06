#include "CitiesGraphReader.hpp"

#include <cassert>
#include <stdexcept>

CitiesMatrix CitiesGraphReader::readFromFile(const std::string &filename, bool junk_in_file)
{
    auto fileStream = openFile(filename);

	std::string junk;
	if (junk_in_file) {
		do {
			fileStream >> junk;
		} while (junk != "DIMENSION:");
	}
	
	size_t numberOfCities;
    fileStream >> numberOfCities;

	if (junk_in_file) {
		for (size_t i = 0; i < 5; i++) {
			fileStream >> junk;
		}
	}

	return readCostsMatrix(fileStream, numberOfCities);
}

std::ifstream CitiesGraphReader::openFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (file.is_open())
        return file;

    throw new std::invalid_argument("File not found: " + filename);
}

CitiesMatrix CitiesGraphReader::readCostsMatrix(std::ifstream &fileStream, const size_t &numberOfCities)
{
	CitiesMatrix cities(numberOfCities);

	for (size_t col = 0; col < numberOfCities; col++) {
		for (size_t row = 0; row < numberOfCities; row++) {
			int weight;
			fileStream >> weight;

			if (col == row) {
				continue;
			}

			cities.connect_cities(col, row, weight);
		}
	}

	return cities;
}
