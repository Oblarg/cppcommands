#include "CommandTestBase.h"
#include "frc/experimental/command/SequentialCommandGroup.h"
#include "frc/experimental/command/InstantCommand.h"

using namespace frc::experimental;

class SequentialCommandGroupTest : public CommandTestBase {

};

TEST_F(SequentialCommandGroupTest, SequentialGroupScheduleTest){
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem subsystem;

  MockCommandHolder command1Holder{true, {&subsystem}};
  std::unique_ptr<MockCommand> command1 = command1Holder.GetMock();
  MockCommandHolder command2Holder{true, {&subsystem}};
  std::unique_ptr<MockCommand> command2 = command2Holder.GetMock();
  MockCommandHolder command3Holder{true, {&subsystem}};
  std::unique_ptr<MockCommand> command3 = command3Holder.GetMock();

  SequentialCommandGroup group({command1, command2, command3});

  EXPECT_CALL(*command1, Initialize());
  EXPECT_CALL(*command1, Execute()).Times(1);
  EXPECT_CALL(*command1, End(false));

  EXPECT_CALL(*command2, Initialize());
  EXPECT_CALL(*command2, Execute()).Times(1);
  EXPECT_CALL(*command2, End(false));

  EXPECT_CALL(*command3, Initialize());
  EXPECT_CALL(*command3, Execute()).Times(1);
  EXPECT_CALL(*command3, End(false));

  scheduler.Schedule(&group);

  command1Holder.SetFinished(true);
  scheduler.Run();
  command2Holder.SetFinished(true);
  scheduler.Run();
  command3Holder.SetFinished(true);
  scheduler.Run();

  EXPECT_FALSE(scheduler.IsScheduled(&group));
}

TEST_F(SequentialCommandGroupTest, SequentialGroupInterruptTest){
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem subsystem;

  MockCommandHolder command1Holder{true, {&subsystem}};
  std::unique_ptr<MockCommand> command1 = command1Holder.GetMock();
  MockCommandHolder command2Holder{true, {&subsystem}};
  std::unique_ptr<MockCommand> command2 = command2Holder.GetMock();
  MockCommandHolder command3Holder{true, {&subsystem}};
  std::unique_ptr<MockCommand> command3 = command3Holder.GetMock();

  SequentialCommandGroup group({command1, command2, command3});

  EXPECT_CALL(*command1, Initialize());
  EXPECT_CALL(*command1, Execute()).Times(1);
  EXPECT_CALL(*command1, End(false));

  EXPECT_CALL(*command2, Initialize());
  EXPECT_CALL(*command2, Execute()).Times(0);
  EXPECT_CALL(*command2, End(false)).Times(0);
  EXPECT_CALL(*command2, End(true));

  EXPECT_CALL(*command3, Initialize()).Times(0);
  EXPECT_CALL(*command3, Execute()).Times(0);
  EXPECT_CALL(*command3, End(false)).Times(0);
  EXPECT_CALL(*command3, End(true)).Times(0);

  scheduler.Schedule(&group);

  command1Holder.SetFinished(true);
  scheduler.Run();
  scheduler.Cancel(&group);
  scheduler.Run();

  EXPECT_FALSE(scheduler.IsScheduled(&group));
}

TEST_F(SequentialCommandGroupTest, SequentialGroupNotScheduledCancelTest){
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem subsystem;

  InstantCommand command1([]{}, {&subsystem});
  InstantCommand command2([]{}, {&subsystem});

  SequentialCommandGroup group({std::make_unique<Command>(command1), 
    std::make_unique<Command>(command2)});

  EXPECT_NO_FATAL_FAILURE(scheduler.Cancel(&group));
}
