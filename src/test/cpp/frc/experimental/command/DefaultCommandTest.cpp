#include "frc/experimental/command/CommandScheduler.h"
#include "frc/experimental/buttons/InternalButton.h"
#include "CommandTestBase.h"

using namespace frc::experimental;

class DefaultCommandTest : public CommandTestBase {
  
};

TEST_F(DefaultCommandTest, DefaultCommandScheduleTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem subsystem;

  MockCommandHolder command1Holder{true, {&subsystem}};
  MockCommandHolder::MockCommand* command1 = command1Holder.GetMock();

  scheduler.SetDefaultCommand(&subsystem, command1);
  scheduler.Run();

  EXPECT_TRUE(scheduler.IsScheduled(command1));
}

TEST_F(DefaultCommandTest, DefaultCommandInterruptResumeTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem subsystem;

  MockCommandHolder command1Holder{true, {&subsystem}};
  MockCommandHolder::MockCommand* command1 = command1Holder.GetMock();
  MockCommandHolder command2Holder{true, {&subsystem}};
  MockCommandHolder::MockCommand* command2 = command2Holder.GetMock();

  scheduler.SetDefaultCommand(&subsystem, command1);
  scheduler.Run();
  scheduler.Schedule(command2);

  EXPECT_TRUE(scheduler.IsScheduled(command2));
  EXPECT_FALSE(scheduler.IsScheduled(command1));

  scheduler.Cancel(command2);
  scheduler.Run();

  EXPECT_TRUE(scheduler.IsScheduled(command1));
}