#pragma once

class ProductionCalculator {
public:
    static int CalcActualProduction(int shortage, double yield);
    static int CalcTotalTime(int avgTime, int actualQty);
};
