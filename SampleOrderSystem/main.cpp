#include "View/ConsoleView.h"
#include "View/MainMenuView.h"
#include "Controller/MenuKey.h"
#include <windows.h>

// Phase 2~6 에서 각 Controller 가 연결됨
// Phase 1-5: 메뉴 UI 동작만 확인
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    ConsoleView  console;
    MainMenuView menuView(console);

    menuView.Show();

    std::string input = console.ReadLine();
    if (input == MenuKey::EXIT) {
        console.PrintLine("시스템을 종료합니다.");
    } else {
        console.PrintError("Phase 1: 세부 기능은 Phase 2 이후 구현됩니다.");
    }

    return 0;
}
