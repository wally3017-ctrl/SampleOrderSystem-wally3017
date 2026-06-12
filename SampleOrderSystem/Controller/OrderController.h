#pragma once
#include "IMenuController.h"
#include "../Model/Order.h"
#include "../Model/Sample.h"
#include "../Repository/IRepository.h"
#include "../Production/IProductionQueue.h"
#include "../View/OrderView.h"
#include <string>

class OrderController : public IMenuController {
    IRepository<Order>&  orderRepo_;
    IRepository<Sample>& sampleRepo_;
    IProductionQueue&    productionQueue_;
    OrderView&           view_;

public:
    OrderController(IRepository<Order>&  orderRepo,
                    IRepository<Sample>& sampleRepo,
                    IProductionQueue&    productionQueue,
                    OrderView&           view);

    Order PlaceOrder(const std::string& sampleId,
                     const std::string& customerName,
                     int quantity);

    void Run() override;
};
