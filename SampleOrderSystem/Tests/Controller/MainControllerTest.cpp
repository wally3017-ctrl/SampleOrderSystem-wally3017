#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include "Controller/MainController.h"
#include "View/ConsoleView.h"
#include "MockMenuController.h"

struct MainTestFixture {
    std::ostringstream out;
    std::istringstream in;
    ConsoleView        console{ out, in };
    MockMenuController sampleMock;
    MockMenuController orderMock;
    MainController     controller{ console };

    MainTestFixture() {
        controller.Register("1", &sampleMock);
        controller.Register("2", &orderMock);
    }
};

TEST(MainControllerTest, MainController_Input1_InvokesSampleController) {
    MainTestFixture f;

    EXPECT_CALL(f.sampleMock, Run()).Times(1);
    EXPECT_CALL(f.orderMock,  Run()).Times(0);

    f.controller.ProcessInput("1");
}

TEST(MainControllerTest, MainController_Input2_InvokesOrderController) {
    MainTestFixture f;

    EXPECT_CALL(f.sampleMock, Run()).Times(0);
    EXPECT_CALL(f.orderMock,  Run()).Times(1);

    f.controller.ProcessInput("2");
}

TEST(MainControllerTest, MainController_Input0_ExitsLoop) {
    MainTestFixture f;

    bool result = f.controller.ProcessInput("0");

    EXPECT_FALSE(result);
}

TEST(MainControllerTest, MainController_InvalidInput_ShowsErrorMessage) {
    MainTestFixture f;

    f.controller.ProcessInput("9");

    EXPECT_FALSE(f.out.str().empty());
}
