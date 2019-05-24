#include "gtest/gtest.h"
#include "frc/experimental/command/CommandScheduler.h"
#include "frc/experimental/buttons/InternalButton.h"
#include "CommandTestBase.h"

using namespace frc::experimental;

class ButtonTest : public CommandTestBase {
  
};

TEST_F(ButtonTest, WhenPressedTest) {
  auto& scheduler = CommandScheduler::GetInstance();
  MockCommandHolder command1Holder{true, {}};
  Command* command1 = command1Holder.GetMock();
  
  InternalButton button;
  button.SetPressed(false);
  auto whenPressed = button.WhenPressed(command1);
  scheduler.Run();
  button.SetPressed(true);
  scheduler.Run();
  scheduler.Run();
}
