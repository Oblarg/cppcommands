#include "gtest/gtest.h"
#include "frc/experimental/command/CommandScheduler.h"
#include "frc/experimental/buttons/InternalButton.h"
#include "CommandTestBase.h"
#include "frc/experimental/command/ParallelCommandGroup.h"

using namespace frc::experimental;

class CommandRequirementsTest : public CommandTestBase {
  
};

TEST_F(CommandRequirementsTest, RequirementInterruptTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem* requirement = new TestSubsystem();

  MockCommandHolder command1Holder{true, {requirement}};
  MockCommandHolder::MockCommand* command1 = command1Holder.GetMock();
  MockCommandHolder command2Holder{true, {requirement}};
  MockCommandHolder::MockCommand* command2 = command2Holder.GetMock();

  EXPECT_CALL(*command1, Initialize());
  EXPECT_CALL(*command1, Execute());
  EXPECT_CALL(*command1, End(true));
  EXPECT_CALL(*command1, End(false)).Times(0);

  EXPECT_CALL(*command2, Initialize());
  EXPECT_CALL(*command2, Execute());
  EXPECT_CALL(*command2, End(true)).Times(0);
  EXPECT_CALL(*command2, End(false)).Times(0);

  scheduler.Schedule(command1);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(command1));
  scheduler.Schedule(command2);
  EXPECT_FALSE(scheduler.IsScheduled(command1));
  EXPECT_TRUE(scheduler.IsScheduled(command2));
  scheduler.Run();
}

TEST_F(CommandRequirementsTest, RequirementUninterruptibleTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem* requirement = new TestSubsystem();

  MockCommandHolder command1Holder{true, {requirement}};
  MockCommandHolder::MockCommand* command1 = command1Holder.GetMock();
  MockCommandHolder command2Holder{true, {requirement}};
  MockCommandHolder::MockCommand* command2 = command2Holder.GetMock();

  EXPECT_CALL(*command1, Initialize());
  EXPECT_CALL(*command1, Execute()).Times(2);
  EXPECT_CALL(*command1, End(true)).Times(0);
  EXPECT_CALL(*command1, End(false)).Times(0);

  EXPECT_CALL(*command2, Initialize()).Times(0);
  EXPECT_CALL(*command2, Execute()).Times(0);
  EXPECT_CALL(*command2, End(true)).Times(0);
  EXPECT_CALL(*command2, End(false)).Times(0);

  scheduler.Schedule(false, command1);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(command1));
  scheduler.Schedule(command2);
  EXPECT_TRUE(scheduler.IsScheduled(command1));
  EXPECT_FALSE(scheduler.IsScheduled(command2));
  scheduler.Run();
}

TEST_F(CommandRequirementsTest, ParallelGroupRequirementTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem* requirement1 = new TestSubsystem();
  TestSubsystem* requirement2 = new TestSubsystem();
  TestSubsystem* requirement3 = new TestSubsystem();
  TestSubsystem* requirement4 = new TestSubsystem();

  MockCommandHolder command1Holder{true, {requirement1, requirement2}};
  MockCommandHolder::MockCommand* command1 = command1Holder.GetMock();
  MockCommandHolder command2Holder{true, {requirement3}};
  MockCommandHolder::MockCommand* command2 = command2Holder.GetMock();
  MockCommandHolder command3Holder{true, {requirement3, requirement4}};
  MockCommandHolder::MockCommand* command3 = command2Holder.GetMock();

  Command* group = new ParallelCommandGroup({command1, command2});

  scheduler.Schedule(group);
  scheduler.Schedule(command3);
  scheduler.Schedule(command1);

  EXPECT_TRUE(scheduler.IsScheduled({command1, command3}));
  EXPECT_FALSE(scheduler.IsScheduled(group));
}
