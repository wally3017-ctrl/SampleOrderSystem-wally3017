#pragma once
#include "../Model/Order.h"
#include "../Repository/IRepository.h"
#include <string>

class OrderNumberGenerator {
    IRepository<Order>& orderRepo_;
public:
    explicit OrderNumberGenerator(IRepository<Order>& orderRepo);
    std::string Generate();
};
