// #include "CommandTestBase.h"
// #include "frc/experimental/command/ParallelCommandGroup.h"
// #include "frc/experimental/command/InstantCommand.h"

// using namespace frc::experimental;

// class ParallelCommandGroupTest : public CommandTestBase {

// };

// TEST_F(ParallelCommandGroupTest, ParallelGroupScheduleTest){
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem subsystem;

//   MockCommandHolder command1Holder{true, {&subsystem}};
//   std::unique_ptr<MockCommandHolder::MockCommand>& command1 = command1Holder.GetMock();
//   MockCommandHolder command2Holder{true, {&subsystem}};
//   std::unique_ptr<MockCommandHolder::MockCommand>& command2 = command2Holder.GetMock();

//   ParallelCommandGroup group({command1, command2});

//   EXPECT_CALL(*command1, Initialize());
//   EXPECT_CALL(*command1, Execute()).Times(1);
//   EXPECT_CALL(*command1, End(false));

//   EXPECT_CALL(*command2, Initialize());
//   EXPECT_CALL(*command2, Execute()).Times(2);
//   EXPECT_CALL(*command2, End(false));

//   scheduler.Schedule(&group);

//   command1Holder.SetFinished(true);
//   scheduler.Run();
//   command2Holder.SetFinished(true);
//   scheduler.Run();

//   EXPECT_FALSE(scheduler.IsScheduled(&group));
// }

// TEST_F(ParallelCommandGroupTest, ParallelGroupInterruptTest){
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem subsystem;

//   MockCommandHolder command1Holder{true, {&subsystem}};
//   std::unique_ptr<MockCommandHolder::MockCommand>& command1 = command1Holder.GetMock();
//   MockCommandHolder command2Holder{true, {&subsystem}};
//   std::unique_ptr<MockCommandHolder::MockCommand>& command2 = command2Holder.GetMock();

//   ParallelCommandGroup group({command1, command2});

//   EXPECT_CALL(*command1, Initialize());
//   EXPECT_CALL(*command1, Execute()).Times(1);
//   EXPECT_CALL(*command1, End(false));


//   EXPECT_CALL(*command2, Initialize());
//   EXPECT_CALL(*command2, Execute()).Times(2);
//   EXPECT_CALL(*command2, End(false)).Times(0);
//   EXPECT_CALL(*command2, End(true));

//   scheduler.Schedule(&group);

//   command1Holder.SetFinished(true);
//   scheduler.Run();
//   scheduler.Run();
//   scheduler.Cancel(&group);

//   EXPECT_FALSE(scheduler.IsScheduled(&group));
// }

// TEST_F(ParallelCommandGroupTest, ParallelGroupNotScheduledCancelTest){
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem subsystem;

//   InstantCommand command1([]{}, {&subsystem});
//   InstantCommand command2([]{}, {&subsystem});

//   ParallelCommandGroup group({std::make_unique<Command>(command1), 
//     std::make_unique<Command>(command)});

//   EXPECT_NO_FATAL_FAILURE(scheduler.Cancel(&group));
// }
