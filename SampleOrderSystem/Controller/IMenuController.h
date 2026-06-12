#pragma once

class IMenuController {
public:
    virtual ~IMenuController() = default;
    virtual void Run() = 0;
};
