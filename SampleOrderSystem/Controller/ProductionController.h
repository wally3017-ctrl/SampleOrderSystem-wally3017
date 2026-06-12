#pragma once
#include "IMenuController.h"
#include "../Model/Order.h"
#include "../Model/Sample.h"
#include "../Production/IProductionQueue.h"
#include "../Repository/IRepository.h"
#include "../View/ProductionView.h"

class ProductionController : public IMenuController {
    IProductionQueue&    queue_;
    IRepository<Order>&  orderRepo_;
    IRepository<Sample>& sampleRepo_;
    ProductionView&      view_;

public:
    ProductionController(IProductionQueue&    queue,
                         IRepository<Order>&  orderRepo,
                         IRepository<Sample>& sampleRepo,
                         ProductionView&      view);

    void Complete();
    void Run() override;
};
