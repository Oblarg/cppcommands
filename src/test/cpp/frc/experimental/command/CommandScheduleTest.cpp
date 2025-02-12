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
  EXPECT_CALL(*command, End(false));

  scheduler.Schedule(command);
  EXPECT_TRUE(scheduler.IsScheduled(command));
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(command));
}

TEST_F(CommandScheduleTest, SingleIterationScheduleTest) {
  CommandScheduler scheduler = GetScheduler();
  MockCommandHolder commandHolder{true, {}};
  MockCommandHolder::MockCommand* command = commandHolder.GetMock();
  
  EXPECT_CALL(*command, Initialize());
  EXPECT_CALL(*command, Execute()).Times(2);
  EXPECT_CALL(*command, End(false));

  scheduler.Schedule(command);
  EXPECT_TRUE(scheduler.IsScheduled(command));
  scheduler.Run();
  commandHolder.SetFinished(true);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(command));
}

TEST_F(CommandScheduleTest, MultiScheduleTest) {
  CommandScheduler scheduler = GetScheduler();
  MockCommandHolder command1Holder{true, {}};
  MockCommandHolder::MockCommand* command1 = command1Holder.GetMock();
  MockCommandHolder command2Holder{true, {}};
  MockCommandHolder::MockCommand* command2 = command2Holder.GetMock();
  MockCommandHolder command3Holder{true, {}};
  MockCommandHolder::MockCommand* command3 = command3Holder.GetMock();
  
  EXPECT_CALL(*command1, Initialize());
  EXPECT_CALL(*command1, Execute()).Times(2);
  EXPECT_CALL(*command1, End(false));

  EXPECT_CALL(*command2, Initialize());
  EXPECT_CALL(*command2, Execute()).Times(3);
  EXPECT_CALL(*command2, End(false));

  EXPECT_CALL(*command3, Initialize());
  EXPECT_CALL(*command3, Execute()).Times(4);
  EXPECT_CALL(*command3, End(false));

  scheduler.Schedule(command1);
  scheduler.Schedule(command2);
  scheduler.Schedule(command3);
  EXPECT_TRUE(scheduler.IsScheduled({command1, command2, command3}));
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled({command1, command2, command3}));
  command1Holder.SetFinished(true);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled({command2, command3}));
  EXPECT_FALSE(scheduler.IsScheduled(command1));
  command2Holder.SetFinished(true);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(command3));
  EXPECT_FALSE(scheduler.IsScheduled({command1, command2}));
  command3Holder.SetFinished(true);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled({command1, command2, command3}));
}

TEST_F(CommandScheduleTest, SchedulerCancelTest) {
  CommandScheduler scheduler = GetScheduler();
  MockCommandHolder commandHolder{true, {}};
  MockCommandHolder::MockCommand* command = commandHolder.GetMock();
  
  EXPECT_CALL(*command, Initialize());
  EXPECT_CALL(*command, Execute());
  EXPECT_CALL(*command, End(false)).Times(0);
  EXPECT_CALL(*command, End(true));


  scheduler.Schedule(command);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(command));
  scheduler.Cancel(command);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(command));
}

TEST_F(CommandScheduleTest, NotScheduledCancelTest) {
  CommandScheduler scheduler = GetScheduler();
  MockCommandHolder commandHolder{true, {}};
  MockCommandHolder::MockCommand* command = commandHolder.GetMock();

  EXPECT_NO_FATAL_FAILURE(scheduler.Cancel(command));
}
