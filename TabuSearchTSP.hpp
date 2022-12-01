#pragma once

#include "TSPAlgorithm.hpp"

class TabuSearchTSP : public TSPAlgorithm {
public:
    TabuSearchTSP(const CitiesMatrix &matrix)
        : TSPAlgorithm(matrix)
    {
    }
    
    TSPResult solve() override;

private:
    

};