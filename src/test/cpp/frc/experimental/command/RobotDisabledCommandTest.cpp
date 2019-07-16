#include "CommandTestBase.h"

using namespace frc::experimental;

class RobotDisabledCommandTest : public CommandTestBase {

};

TEST_F(RobotDisabledCommandTest, RobotDisabledCommandCancelTest) {
  CommandScheduler scheduler = GetScheduler();

  MockCommand command({}, false, false);

  EXPECT_CALL(command, End(true));

  SetDSEnabled(true);

  scheduler.Schedule(&command);
  scheduler.Run();

  EXPECT_TRUE(scheduler.IsScheduled(&command));

  SetDSEnabled(false);

  scheduler.Run();

  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(RobotDisabledCommandTest, RunWhenDisabledTest) {
  CommandScheduler scheduler = GetScheduler();

  MockCommand command1;
  MockCommand command2;

  scheduler.Schedule(&command1);

  SetDSEnabled(false);

  scheduler.Run();

  scheduler.Schedule(&command2);

  std::cout << (&command2)->RunsWhenDisabled();
  std::cout << "\n \n";

  EXPECT_TRUE(scheduler.IsScheduled(&command1));
  EXPECT_TRUE(scheduler.IsScheduled(&command2));
}