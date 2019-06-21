#pragma once

#include "gtest/gtest.h"
#include "frc/experimental/command/CommandScheduler.h"
#include "frc/experimental/buttons/InternalButton.h"
#include "CommandTestBase.h"
#include "frc/experimental/command/InstantCommand.h"
#include "frc/experimental/command/ParallelCommandGroup.h"
#include "frc/experimental/command/ParallelRaceGroup.h"
#include "frc/experimental/command/ParallelDeadlineGroup.h"
#include "frc/experimental/command/ConditionalCommand.h"
#include "frc/experimental/command/SelectCommand.h"
#include "frc/experimental/command/SequentialCommandGroup.h"

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

  Command* command1 = new InstantCommand([]{},{requirement1, requirement2});
  Command* command2 = new InstantCommand([]{},{requirement3});
  Command* command3 = new InstantCommand([]{},{requirement3, requirement4});

  Command* group = new ParallelCommandGroup({command1, command2});

  scheduler.Schedule(group);
  scheduler.Schedule(command3);

  EXPECT_TRUE(scheduler.IsScheduled({command3}));
  EXPECT_FALSE(scheduler.IsScheduled(group));
}

TEST_F(CommandRequirementsTest, SequentialGroupRequirementTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem* requirement1 = new TestSubsystem();
  TestSubsystem* requirement2 = new TestSubsystem();
  TestSubsystem* requirement3 = new TestSubsystem();
  TestSubsystem* requirement4 = new TestSubsystem();

  Command* command1 = new InstantCommand([]{},{requirement1, requirement2});
  Command* command2 = new InstantCommand([]{},{requirement3});
  Command* command3 = new InstantCommand([]{},{requirement3, requirement4});

  Command* group = new SequentialCommandGroup({command1, command2});

  scheduler.Schedule(group);
  scheduler.Schedule(command3);

  EXPECT_TRUE(scheduler.IsScheduled({command3}));
  EXPECT_FALSE(scheduler.IsScheduled(group));
}

TEST_F(CommandRequirementsTest, RaceGroupRequirementTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem* requirement1 = new TestSubsystem();
  TestSubsystem* requirement2 = new TestSubsystem();
  TestSubsystem* requirement3 = new TestSubsystem();
  TestSubsystem* requirement4 = new TestSubsystem();

  Command* command1 = new InstantCommand([]{},{requirement1, requirement2});
  Command* command2 = new InstantCommand([]{},{requirement3});
  Command* command3 = new InstantCommand([]{},{requirement3, requirement4});

  Command* group = new ParallelRaceGroup({command1, command2});

  scheduler.Schedule(group);
  scheduler.Schedule(command3);

  EXPECT_TRUE(scheduler.IsScheduled({command3}));
  EXPECT_FALSE(scheduler.IsScheduled(group));
}

TEST_F(CommandRequirementsTest, ParallelDeadlineRequirementTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem* requirement1 = new TestSubsystem();
  TestSubsystem* requirement2 = new TestSubsystem();
  TestSubsystem* requirement3 = new TestSubsystem();
  TestSubsystem* requirement4 = new TestSubsystem();

  Command* command1 = new InstantCommand([]{},{requirement1, requirement2});
  Command* command2 = new InstantCommand([]{},{requirement3});
  Command* command3 = new InstantCommand([]{},{requirement3, requirement4});

  Command* group = new ParallelDeadlineGroup({command1, command2});

  scheduler.Schedule(group);
  scheduler.Schedule(command3);

  EXPECT_TRUE(scheduler.IsScheduled({command3}));
  EXPECT_FALSE(scheduler.IsScheduled(group));
}

TEST_F(CommandRequirementsTest, ConditionalCommandRequirementTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem* requirement1 = new TestSubsystem();
  TestSubsystem* requirement2 = new TestSubsystem();
  TestSubsystem* requirement3 = new TestSubsystem();
  TestSubsystem* requirement4 = new TestSubsystem();

  Command* command1 = new InstantCommand([]{},{requirement1, requirement2});
  Command* command2 = new InstantCommand([]{},{requirement3});
  Command* command3 = new InstantCommand([]{},{requirement3, requirement4});

  Command* conditional = new ConditionalCommand(command1, command2, []{return true;});

  scheduler.Schedule(conditional);
  scheduler.Schedule(command3);

  EXPECT_TRUE(scheduler.IsScheduled({command3}));
  EXPECT_FALSE(scheduler.IsScheduled(conditional));
}

TEST_F(CommandRequirementsTest, SelectCommandRequirementTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem* requirement1 = new TestSubsystem();
  TestSubsystem* requirement2 = new TestSubsystem();
  TestSubsystem* requirement3 = new TestSubsystem();
  TestSubsystem* requirement4 = new TestSubsystem();

  Command* command1 = new InstantCommand([]{},{requirement1, requirement2});
  Command* command2 = new InstantCommand([]{},{requirement3});
  Command* command3 = new InstantCommand([]{},{requirement3, requirement4});

  int key1 = 1;
  int key2 = 2;

  Command* select = new SelectCommand({{&key1, command1}, {&key2, command2}}, [&key1]{return &key1;});

  scheduler.Schedule(select);
  scheduler.Schedule(command3);

  EXPECT_TRUE(scheduler.IsScheduled({command3}));
  EXPECT_FALSE(scheduler.IsScheduled(select));
}
