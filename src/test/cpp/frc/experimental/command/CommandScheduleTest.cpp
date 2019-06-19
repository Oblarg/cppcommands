#include "gtest/gtest.h"
#include "frc/experimental/command/CommandScheduler.h"
#include "frc/experimental/buttons/InternalButton.h"
#include "CommandTestBase.h"

using namespace frc::experimental;

class CommandScheduleTest : public CommandTestBase {
  
};

TEST_F(CommandScheduleTest, InstantScheduleTest) {
  CommandScheduler scheduler = GetScheduler();
  MockCommandHolder commandHolder{true, {}};
  commandHolder.SetFinished(true);
  MockCommandHolder::MockCommand* command = commandHolder.GetMock();
  
  EXPECT_CALL(*command, Initialize());
  EXPECT_CALL(*command, Execute());
  EXPECT_CALL(*command, End(true));

  scheduler.Schedule(command);
  EXPECT_TRUE(scheduler.IsScheduled(command));
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(command));
}
