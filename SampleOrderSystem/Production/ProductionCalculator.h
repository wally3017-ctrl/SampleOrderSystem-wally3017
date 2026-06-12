#pragma once

class ProductionCalculator {
public:
    static constexpr double kYieldSafetyFactor = 0.9;

    static int CalcActualProduction(int shortage, double yield);
    static int CalcTotalTime(int avgTime, int actualQty);
};
