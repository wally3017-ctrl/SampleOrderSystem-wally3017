#include "View/ConsoleView.h"
#include "View/MainMenuView.h"
#include "View/SampleView.h"
#include "View/OrderView.h"
#include "View/ProductionView.h"
#include "View/MonitoringView.h"
#include "Controller/IMenuController.h"
#include "Controller/MonitoringService.h"
#include "Controller/MonitoringController.h"
#include "Controller/MainController.h"
#include "Controller/SampleController.h"
#include "Controller/OrderController.h"
#include "Controller/ProductionController.h"
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
    ProductionQueue        productionQueue("production_queue.json");

    SampleView     sampleView(console);
    OrderView      orderView(console);
    ProductionView productionView(console);
    MonitoringView monitoringView(console);

    SampleController     sampleCtrl(sampleRepo, sampleView);
    OrderController      orderCtrl(orderRepo, sampleRepo, productionQueue, orderView);
    ProductionController productionCtrl(productionQueue, orderRepo, sampleRepo, productionView);

    MonitoringService    monitoringSvc(orderRepo, sampleRepo);
    MonitoringController monitorCtrl(monitoringSvc, monitoringView);
    StubMenuController   releaseCtrl(console, "출고 처리");

    MainMenuView   menuView(console);
    MainController mainCtrl(&sampleCtrl, &orderCtrl, console);

    std::string input;
    while (true) {
        menuView.Show();
        input = console.ReadLine();
        if (input == "0") {
            console.PrintLine("시스템을 종료합니다.");
            break;
        } else if (input == "4") {
            monitorCtrl.Run();
        } else if (input == "5") {
            productionCtrl.Run();
        } else if (input == "6") {
            releaseCtrl.Run();
        } else if (!mainCtrl.ProcessInput(input)) {
            console.PrintLine("시스템을 종료합니다.");
            break;
        }
    }

    return 0;
}
