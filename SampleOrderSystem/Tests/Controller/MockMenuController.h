#pragma once
#include <gmock/gmock.h>
#include "Controller/IMenuController.h"

class MockMenuController : public IMenuController {
public:
    MOCK_METHOD(void, Run, (), (override));
};
