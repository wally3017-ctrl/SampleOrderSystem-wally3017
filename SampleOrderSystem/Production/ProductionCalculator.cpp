#include "ProductionCalculator.h"
#include <cmath>

int ProductionCalculator::CalcActualProduction(int shortage, double yield) {
    if (shortage <= 0) return 0;
    return static_cast<int>(std::ceil(shortage / (yield * kYieldSafetyFactor)));
}

int ProductionCalculator::CalcTotalTime(int avgTime, int actualQty) {
    return avgTime * actualQty;
}
