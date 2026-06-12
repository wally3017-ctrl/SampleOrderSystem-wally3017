#pragma once
#include "StockStatus.h"
#include <string>

struct SampleStockInfo {
    std::string sampleId;
    std::string sampleName;
    int         stock;
    int         orderedQty;
    StockStatus status;
};
