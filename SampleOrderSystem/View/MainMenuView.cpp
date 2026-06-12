#include "MainMenuView.h"

MainMenuView::MainMenuView(ConsoleView& console)
    : console_(console) {}

void MainMenuView::Show(const SystemSummary& summary) {
    console_.PrintSeparator();
    console_.PrintLine("       반도체 시료 생산주문관리 시스템 (S-Semi)");
    console_.PrintSeparator();
    console_.PrintLine(
        " [현황]  시료: "          + std::to_string(summary.sampleCount)          + "건"  +
        "  |  총재고: "            + std::to_string(summary.totalStock)            + "개"  +
        "  |  주문: "              + std::to_string(summary.totalOrders)           + "건"  +
        "  |  생산대기: "          + std::to_string(summary.productionQueueCount)  + "건"
    );
    console_.PrintLine();
    console_.PrintLine(" 1. 시료 관리");
    console_.PrintLine(" 2. 시료 주문");
    console_.PrintLine(" 3. 주문 승인/거절");
    console_.PrintLine(" 4. 모니터링");
    console_.PrintLine(" 5. 생산라인 조회");
    console_.PrintLine(" 6. 출고 처리");
    console_.PrintLine(" 0. 종료");
    console_.PrintSeparator();
    console_.Print(" 메뉴 선택 >> ");
}
