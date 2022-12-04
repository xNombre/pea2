#pragma once

#include "CoolingFunc.hpp"

class LogaritmicCooling : public CoolingFunc {
public:
    void operator()(double &temperature, const double &factor) override
    {
        temperature = temperature / (1 + factor);
    }
};