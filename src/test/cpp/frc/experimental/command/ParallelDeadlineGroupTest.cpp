// #include "CommandTestBase.h"
// #include "frc/experimental/command/ParallelDeadlineGroup.h"
// #include "frc/experimental/command/InstantCommand.h"

// using namespace frc::experimental;

// class ParallelDeadlineGroupTest : public CommandTestBase {

// };

// TEST_F(ParallelDeadlineGroupTest, SequentialGroupScheduleTest){
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem subsystem;

//   MockCommandHolder command1Holder{true, {&subsystem}};
//   std::unique_ptr<MockCommandHolder::MockCommand>& command1 = command1Holder.GetMock();
//   MockCommandHolder command2Holder{true, {&subsystem}};
//   std::unique_ptr<MockCommandHolder::MockCommand>& command2 = command2Holder.GetMock();
//   MockCommandHolder command3Holder{true, {&subsystem}};
//   std::unique_ptr<MockCommandHolder::MockCommand>& command3 = command3Holder.GetMock();

//   ParallelDeadlineGroup group(command1, {command2, command3});

//   EXPECT_CALL(*command1, Initialize());
//   EXPECT_CALL(*command1, Execute()).Times(2);
//   EXPECT_CALL(*command1, End(false));

//   EXPECT_CALL(*command2, Initialize());
//   EXPECT_CALL(*command2, Execute()).Times(1);
//   EXPECT_CALL(*command2, End(false));

//   EXPECT_CALL(*command3, Initialize());
//   EXPECT_CALL(*command3, Execute()).Times(2);
//   EXPECT_CALL(*command3, End(true));

//   scheduler.Schedule(&group);

//   command2Holder.SetFinished(true);
//   scheduler.Run();
//   command1Holder.SetFinished(true);
//   scheduler.Run();

//   EXPECT_FALSE(scheduler.IsScheduled(&group));
// }

// TEST_F(ParallelDeadlineGroupTest, SequentialGroupInterruptTest){
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem subsystem;

//   MockCommandHolder command1Holder{true, {&subsystem}};
//   std::unique_ptr<MockCommandHolder::MockCommand>& command1 = command1Holder.GetMock();
//   MockCommandHolder command2Holder{true, {&subsystem}};
//   std::unique_ptr<MockCommandHolder::MockCommand>& command2 = command2Holder.GetMock();
//   MockCommandHolder command3Holder{true, {&subsystem}};
//   std::unique_ptr<MockCommandHolder::MockCommand>& command3 = command3Holder.GetMock();

//   ParallelDeadlineGroup group(command1, {command2, command3});

//   EXPECT_CALL(*command1, Initialize());
//   EXPECT_CALL(*command1, Execute()).Times(1);
//   EXPECT_CALL(*command1, End(true));

//   EXPECT_CALL(*command2, Initialize());
//   EXPECT_CALL(*command2, Execute()).Times(1);
//   EXPECT_CALL(*command2, End(true));

//   EXPECT_CALL(*command3, Initialize());
//   EXPECT_CALL(*command3, Execute()).Times(1);
//   EXPECT_CALL(*command3, End(true));

//   scheduler.Schedule(&group);

//   scheduler.Run();
//   scheduler.Cancel(&group);
//   scheduler.Run();

//   EXPECT_FALSE(scheduler.IsScheduled(&group));
// }

// TEST_F(ParallelDeadlineGroupTest, SequentialGroupNotScheduledCancelTest){
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem subsystem;

//   InstantCommand command1([]{}, {&subsystem});
//   InstantCommand command2([]{}, {&subsystem});

//   ParallelDeadlineGroup group(std::make_unique<Command>(command1), 
//     {std::make_unique<Command>(command)});

//   EXPECT_NO_FATAL_FAILURE(scheduler.Cancel(&group));
// }
