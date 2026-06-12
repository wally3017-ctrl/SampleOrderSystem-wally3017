#include "View/ConsoleView.h"
#include "View/MainMenuView.h"
#include "View/SampleView.h"
#include "View/OrderView.h"
#include "Controller/IMenuController.h"
#include "Controller/MainController.h"
#include "Controller/SampleController.h"
#include "Controller/OrderController.h"
#include "Repository/JsonRepository.h"
#include "Model/Sample.h"
#include "Model/Order.h"
#include "Production/ProductionQueue.h"
#include <windows.h>

class StubMenuController : public IMenuController {
    ConsoleView& console_;
    std::string  label_;
public:
    StubMenuController(ConsoleView& console, std::string label)
        : console_(console), label_(std::move(label)) {}
    void Run() override {
        console_.PrintLine(" [" + label_ + "] 기능은 추후 구현 예정입니다.");
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    ConsoleView console;

    JsonRepository<Sample> sampleRepo("samples.json");
    JsonRepository<Order>  orderRepo("orders.json");
    ProductionQueue        productionQueue;

    SampleView sampleView(console);
    OrderView  orderView(console);

    SampleController   sampleCtrl(sampleRepo, sampleView);
    OrderController    orderCtrl(orderRepo, sampleRepo, productionQueue, orderView);

    MainMenuView   menuView(console);
    MainController mainCtrl(&sampleCtrl, &orderCtrl, console);

    std::string input;
    while (true) {
        menuView.Show();
        input = console.ReadLine();
        if (!mainCtrl.ProcessInput(input)) {
            console.PrintLine("시스템을 종료합니다.");
            break;
        }
    }

    return 0;
}
