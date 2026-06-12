#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include "Controller/MainController.h"
#include "MockMenuController.h"


// Phase 1-3 [Red]: MainController 스텁으로 인해 아래 테스트 모두 실패

TEST(MainControllerTest, MainController_Input1_InvokesSampleController) {
    MockMenuController sampleMock;
    MockMenuController orderMock;
    std::ostringstream out;
    MainController controller(&sampleMock, &orderMock, out);

    EXPECT_CALL(sampleMock, Run()).Times(1);
    EXPECT_CALL(orderMock,  Run()).Times(0);

    controller.ProcessInput("1");
}

TEST(MainControllerTest, MainController_Input2_InvokesOrderController) {
    MockMenuController sampleMock;
    MockMenuController orderMock;
    std::ostringstream out;
    MainController controller(&sampleMock, &orderMock, out);

    EXPECT_CALL(sampleMock, Run()).Times(0);
    EXPECT_CALL(orderMock,  Run()).Times(1);

    controller.ProcessInput("2");
}

TEST(MainControllerTest, MainController_Input0_ExitsLoop) {
    MockMenuController sampleMock;
    MockMenuController orderMock;
    std::ostringstream out;
    MainController controller(&sampleMock, &orderMock, out);

    bool result = controller.ProcessInput("0");

    EXPECT_FALSE(result); // false = 루프 종료
}

TEST(MainControllerTest, MainController_InvalidInput_ShowsErrorMessage) {
    MockMenuController sampleMock;
    MockMenuController orderMock;
    std::ostringstream out;
    MainController controller(&sampleMock, &orderMock, out);

    controller.ProcessInput("9");

    EXPECT_FALSE(out.str().empty()); // 오류 메시지가 출력에 기록돼야 함
}
