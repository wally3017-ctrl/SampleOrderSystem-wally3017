#pragma once
#include "IMenuController.h"
#include "../Model/Sample.h"
#include "../Repository/IRepository.h"

class SampleController : public IMenuController {
    IRepository<Sample>& repo_;

public:
    explicit SampleController(IRepository<Sample>& repo);

    void Register(const Sample& sample);

    void Run() override;
};
