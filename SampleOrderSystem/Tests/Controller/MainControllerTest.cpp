#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include "Controller/MainController.h"
#include "View/ConsoleView.h"
#include "MockMenuController.h"


TEST(MainControllerTest, MainController_Input1_InvokesSampleController) {
    MockMenuController sampleMock;
    MockMenuController orderMock;
    std::ostringstream out;
    std::istringstream in;
    ConsoleView console(out, in);
    MainController controller(&sampleMock, &orderMock, console);

    EXPECT_CALL(sampleMock, Run()).Times(1);
    EXPECT_CALL(orderMock,  Run()).Times(0);

    controller.ProcessInput("1");
}

TEST(MainControllerTest, MainController_Input2_InvokesOrderController) {
    MockMenuController sampleMock;
    MockMenuController orderMock;
    std::ostringstream out;
    std::istringstream in;
    ConsoleView console(out, in);
    MainController controller(&sampleMock, &orderMock, console);

    EXPECT_CALL(sampleMock, Run()).Times(0);
    EXPECT_CALL(orderMock,  Run()).Times(1);

    controller.ProcessInput("2");
}

TEST(MainControllerTest, MainController_Input0_ExitsLoop) {
    MockMenuController sampleMock;
    MockMenuController orderMock;
    std::ostringstream out;
    std::istringstream in;
    ConsoleView console(out, in);
    MainController controller(&sampleMock, &orderMock, console);

    bool result = controller.ProcessInput("0");

    EXPECT_FALSE(result);
}

TEST(MainControllerTest, MainController_InvalidInput_ShowsErrorMessage) {
    MockMenuController sampleMock;
    MockMenuController orderMock;
    std::ostringstream out;
    std::istringstream in;
    ConsoleView console(out, in);
    MainController controller(&sampleMock, &orderMock, console);

    controller.ProcessInput("9");

    EXPECT_FALSE(out.str().empty());
}
