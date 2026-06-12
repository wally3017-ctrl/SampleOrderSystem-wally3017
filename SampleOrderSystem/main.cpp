#include "View/ConsoleView.h"
#include "View/MainMenuView.h"
#include "View/SampleView.h"
#include "Controller/IMenuController.h"
#include "Controller/MainController.h"
#include "Controller/SampleController.h"
#include "Repository/JsonRepository.h"
#include "Model/Sample.h"
#include <windows.h>

// 아직 구현되지 않은 메뉴 컨트롤러 플레이스홀더
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

    SampleView sampleView(console);

    SampleController   sampleCtrl(sampleRepo, sampleView);
    StubMenuController orderCtrl(console, "시료 주문");   // Phase 3에서 교체 예정

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
