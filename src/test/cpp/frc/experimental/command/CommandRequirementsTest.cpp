// #include "frc/experimental/command/CommandScheduler.h"
// #include "frc/experimental/buttons/InternalButton.h"
// #include "CommandTestBase.h"
// #include "frc/experimental/command/InstantCommand.h"
// #include "frc/experimental/command/ParallelCommandGroup.h"
// #include "frc/experimental/command/ParallelRaceGroup.h"
// #include "frc/experimental/command/ParallelDeadlineGroup.h"
// #include "frc/experimental/command/ConditionalCommand.h"
// #include "frc/experimental/command/SelectCommand.h"
// #include "frc/experimental/command/SequentialCommandGroup.h"

// using namespace frc::experimental;

// class CommandRequirementsTest : public CommandTestBase {
  
// };

// TEST_F(CommandRequirementsTest, RequirementInterruptTest) {
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem requirement;

//   MockCommandHolder command1Holder{true, {&requirement}};
//   std::unique_ptr<MockCommand> command1 = command1Holder.GetMock();
//   MockCommandHolder command2Holder{true, {&requirement}};
//   std::unique_ptr<MockCommand> command2 = command2Holder.GetMock();

//   EXPECT_CALL(*command1, Initialize());
//   EXPECT_CALL(*command1, Execute());
//   EXPECT_CALL(*command1, End(true));
//   EXPECT_CALL(*command1, End(false)).Times(0);

//   EXPECT_CALL(*command2, Initialize());
//   EXPECT_CALL(*command2, Execute());
//   EXPECT_CALL(*command2, End(true)).Times(0);
//   EXPECT_CALL(*command2, End(false)).Times(0);

//   scheduler.Schedule(command1);
//   scheduler.Run();
//   EXPECT_TRUE(scheduler.IsScheduled(command1));
//   scheduler.Schedule(command2);
//   EXPECT_FALSE(scheduler.IsScheduled(command1));
//   EXPECT_TRUE(scheduler.IsScheduled(command2));
//   scheduler.Run();
// }

// TEST_F(CommandRequirementsTest, RequirementUninterruptibleTest) {
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem requirement;

//   MockCommandHolder command1Holder{true, {&requirement}};
//   std::unique_ptr<MockCommand> command1 = command1Holder.GetMock();
//   MockCommandHolder command2Holder{true, {&requirement}};
//   std::unique_ptr<MockCommand> command2 = command2Holder.GetMock();

//   EXPECT_CALL(*command1, Initialize());
//   EXPECT_CALL(*command1, Execute()).Times(2);
//   EXPECT_CALL(*command1, End(true)).Times(0);
//   EXPECT_CALL(*command1, End(false)).Times(0);

//   EXPECT_CALL(*command2, Initialize()).Times(0);
//   EXPECT_CALL(*command2, Execute()).Times(0);
//   EXPECT_CALL(*command2, End(true)).Times(0);
//   EXPECT_CALL(*command2, End(false)).Times(0);

//   scheduler.Schedule(false, command1);
//   scheduler.Run();
//   EXPECT_TRUE(scheduler.IsScheduled(command1));
//   scheduler.Schedule(command2);
//   EXPECT_TRUE(scheduler.IsScheduled(command1));
//   EXPECT_FALSE(scheduler.IsScheduled(command2));
//   scheduler.Run();
// }

// TEST_F(CommandRequirementsTest, ParallelGroupRequirementTest) {
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem requirement1;
//   TestSubsystem requirement2;
//   TestSubsystem requirement3;
//   TestSubsystem requirement4;

//   Command* command1 = new InstantCommand([]{},{&requirement1, &requirement2});
//   Command* command2 = new InstantCommand([]{},{&requirement3});
//   Command* command3 = new InstantCommand([]{},{&requirement3, &requirement4});

//   Command* group = new ParallelCommandGroup({command1, command2});

//   scheduler.Schedule(group);
//   scheduler.Schedule(command3);

//   EXPECT_TRUE(scheduler.IsScheduled({command3}));
//   EXPECT_FALSE(scheduler.IsScheduled(group));
// }

// TEST_F(CommandRequirementsTest, SequentialGroupRequirementTest) {
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem requirement1;
//   TestSubsystem requirement2;
//   TestSubsystem requirement3;
//   TestSubsystem requirement4;

//   Command* command1 = new InstantCommand([]{},{&requirement1, &requirement2});
//   Command* command2 = new InstantCommand([]{},{&requirement3});
//   Command* command3 = new InstantCommand([]{},{&requirement3, &requirement4});

//   Command* group = new SequentialCommandGroup({command1, command2});

//   scheduler.Schedule(group);
//   scheduler.Schedule(command3);

//   EXPECT_TRUE(scheduler.IsScheduled({command3}));
//   EXPECT_FALSE(scheduler.IsScheduled(group));
// }

// TEST_F(CommandRequirementsTest, RaceGroupRequirementTest) {
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem requirement1;
//   TestSubsystem requirement2;
//   TestSubsystem requirement3;
//   TestSubsystem requirement4;

//   Command* command1 = new InstantCommand([]{},{&requirement1, &requirement2});
//   Command* command2 = new InstantCommand([]{},{&requirement3});
//   Command* command3 = new InstantCommand([]{},{&requirement3, &requirement4});

//   Command* group = new ParallelRaceGroup({command1, command2});

//   scheduler.Schedule(group);
//   scheduler.Schedule(command3);

//   EXPECT_TRUE(scheduler.IsScheduled({command3}));
//   EXPECT_FALSE(scheduler.IsScheduled(group));
// }

// TEST_F(CommandRequirementsTest, ParallelDeadlineRequirementTest) {
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem requirement1;
//   TestSubsystem requirement2;
//   TestSubsystem requirement3;
//   TestSubsystem requirement4;

//   Command* command1 = new InstantCommand([]{},{&requirement1, &requirement2});
//   Command* command2 = new InstantCommand([]{},{&requirement3});
//   Command* command3 = new InstantCommand([]{},{&requirement3, &requirement4});

//   Command* group = new ParallelDeadlineGroup(std::make_unique<Command>(command1), 
//     {std::make_unique<Command>(command2)});

//   scheduler.Schedule(group);
//   scheduler.Schedule(command3);

//   EXPECT_TRUE(scheduler.IsScheduled({command3}));
//   EXPECT_FALSE(scheduler.IsScheduled(group));
// }

// TEST_F(CommandRequirementsTest, ConditionalCommandRequirementTest) {
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem requirement1;
//   TestSubsystem requirement2;
//   TestSubsystem requirement3;
//   TestSubsystem requirement4;

//   Command* command1 = new InstantCommand([]{},{&requirement1, &requirement2});
//   Command* command2 = new InstantCommand([]{},{&requirement3});
//   Command* command3 = new InstantCommand([]{},{&requirement3, &requirement4});

//   Command* conditional = new ConditionalCommand(
//     std::make_unique<Command>(command1), 
//     std::make_unique<Command>(command2), []{return true;});

//   scheduler.Schedule(conditional);
//   scheduler.Schedule(command3);

//   EXPECT_TRUE(scheduler.IsScheduled({command3}));
//   EXPECT_FALSE(scheduler.IsScheduled(conditional));
// }

// TEST_F(CommandRequirementsTest, SelectCommandRequirementTest) {
//   CommandScheduler scheduler = GetScheduler();

//   TestSubsystem requirement1;
//   TestSubsystem requirement2;
//   TestSubsystem requirement3;
//   TestSubsystem requirement4;

//   Command* command1 = new InstantCommand([]{},{&requirement1, &requirement2});
//   Command* command2 = new InstantCommand([]{},{&requirement3});
//   Command* command3 = new InstantCommand([]{},{&requirement3, &requirement4});

//   Command* select = new SelectCommand<int>(
//     {{1, std::make_unique<Command>(command1)}, 
//     {2, std::make_unique<Command>(command2)}}, []{return 1;});

//   scheduler.Schedule(select);
//   scheduler.Schedule(command3);

//   EXPECT_TRUE(scheduler.IsScheduled({command3}));
//   EXPECT_FALSE(scheduler.IsScheduled(select));
// }

// // TEST_F(CommandRequirementsTest, DefaultCommandRequirementErrorTest) {
// //   TestSubsystem requirement1;
// //   ErrorConfirmer confirmer("require");

// //   MockCommandHolder command1Holder{true, {}};
// //   std::unique_ptr<MockCommand> command1 = command1Holder.GetMock();
  
// //   //TODO: actually test for correct error triggering
// //   requirement1.SetDefaultCommand(command1);

// //   EXPECT_TRUE(requirement1.GetDefaultCommand() == NULL);
// // }
