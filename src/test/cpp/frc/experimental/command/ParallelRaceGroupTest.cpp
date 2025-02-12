#include "CommandTestBase.h"
#include "frc/experimental/command/ParallelRaceGroup.h"
#include "frc/experimental/command/InstantCommand.h"

using namespace frc::experimental;

class ParallelRaceGroupTest : public CommandTestBase {

};

TEST_F(ParallelRaceGroupTest, SequentialGroupScheduleTest){
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem subsystem;

  MockCommandHolder command1Holder{true, {&subsystem}};
  MockCommandHolder::MockCommand* command1 = command1Holder.GetMock();
  MockCommandHolder command2Holder{true, {&subsystem}};
  MockCommandHolder::MockCommand* command2 = command2Holder.GetMock();
  MockCommandHolder command3Holder{true, {&subsystem}};
  MockCommandHolder::MockCommand* command3 = command3Holder.GetMock();

  ParallelRaceGroup group({command1, command2, command3});

  EXPECT_CALL(*command1, Initialize());
  EXPECT_CALL(*command1, Execute()).Times(2);
  EXPECT_CALL(*command1, End(true));

  EXPECT_CALL(*command2, Initialize());
  EXPECT_CALL(*command2, Execute()).Times(2);
  EXPECT_CALL(*command2, End(false));

  EXPECT_CALL(*command3, Initialize());
  EXPECT_CALL(*command3, Execute()).Times(2);
  EXPECT_CALL(*command3, End(true));

  scheduler.Schedule(&group);

  scheduler.Run();
  command2Holder.SetFinished(true);
  scheduler.Run();

  EXPECT_FALSE(scheduler.IsScheduled(&group));
}

TEST_F(ParallelRaceGroupTest, SequentialGroupInterruptTest){
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem subsystem;

  MockCommandHolder command1Holder{true, {&subsystem}};
  MockCommandHolder::MockCommand* command1 = command1Holder.GetMock();
  MockCommandHolder command2Holder{true, {&subsystem}};
  MockCommandHolder::MockCommand* command2 = command2Holder.GetMock();
  MockCommandHolder command3Holder{true, {&subsystem}};
  MockCommandHolder::MockCommand* command3 = command3Holder.GetMock();

  ParallelRaceGroup group({command1, command2, command3});

  EXPECT_CALL(*command1, Initialize());
  EXPECT_CALL(*command1, Execute()).Times(1);
  EXPECT_CALL(*command1, End(true));

  EXPECT_CALL(*command2, Initialize());
  EXPECT_CALL(*command2, Execute()).Times(1);
  EXPECT_CALL(*command2, End(true));

  EXPECT_CALL(*command3, Initialize());
  EXPECT_CALL(*command3, Execute()).Times(1);
  EXPECT_CALL(*command3, End(true));

  scheduler.Schedule(&group);

  scheduler.Run();
  scheduler.Cancel(&group);
  scheduler.Run();

  EXPECT_FALSE(scheduler.IsScheduled(&group));
}

TEST_F(ParallelRaceGroupTest, SequentialGroupNotScheduledCancelTest){
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem subsystem;

  InstantCommand command1([]{}, {&subsystem});
  InstantCommand command2([]{}, {&subsystem});

  ParallelRaceGroup group({&command1, &command2});

  EXPECT_NO_FATAL_FAILURE(scheduler.Cancel(&group));
}
