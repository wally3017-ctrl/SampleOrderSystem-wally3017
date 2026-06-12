#include <gtest/gtest.h>
#include "Production/ProductionCalculator.h"

// 공식: ceil(부족분 / (수율 × 0.9))

TEST(ProductionCalculatorTest, CalcActualProduction_NormalYield_ReturnsCorrectCeil) {
    // ceil(170 / (0.92 × 0.9)) = ceil(170 / 0.828) = ceil(205.31) = 206
    EXPECT_EQ(ProductionCalculator::CalcActualProduction(170, 0.92), 206);
}

TEST(ProductionCalculatorTest, CalcActualProduction_HighYield_ReturnsMinimumNeeded) {
    // ceil(10 / (1.0 × 0.9)) = ceil(11.11) = 12
    EXPECT_EQ(ProductionCalculator::CalcActualProduction(10, 1.0), 12);
}

TEST(ProductionCalculatorTest, CalcActualProduction_LowYield_ReturnsLargerQuantity) {
    // ceil(10 / (0.5 × 0.9)) = ceil(10 / 0.45) = ceil(22.22) = 23
    EXPECT_EQ(ProductionCalculator::CalcActualProduction(10, 0.5), 23);
}

TEST(ProductionCalculatorTest, CalcActualProduction_ShortageIsZero_ReturnsZero) {
    EXPECT_EQ(ProductionCalculator::CalcActualProduction(0, 0.9), 0);
}

TEST(ProductionCalculatorTest, CalcTotalProductionTime_ReturnsAvgTimeMultipliedByActual) {
    EXPECT_EQ(ProductionCalculator::CalcTotalTime(10, 206), 2060);
}
