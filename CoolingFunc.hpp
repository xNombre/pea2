#pragma once

class CoolingFunc {
public:
    virtual void operator()(double &temperature, const double &factor) = 0;
};