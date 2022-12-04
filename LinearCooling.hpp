#pragma once

#include "CoolingFunc.hpp"

class LinearCooling : public CoolingFunc {
public:
    void operator()(double& temperature, const double &factor) override
    {
        temperature -= factor;
    }
};