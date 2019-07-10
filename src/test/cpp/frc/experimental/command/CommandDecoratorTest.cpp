#include "CommandTestBase.h"
#include "frc/experimental/command/RunCommand.h"
#include "frc/experimental/command/ParallelRaceGroup.h"

using namespace frc::experimental;

class CommandDecoratorTest : public CommandTestBase {
  
};

TEST_F(CommandDecoratorTest, WithTimeoutTest) {
  CommandScheduler scheduler = GetScheduler();

  auto command = RunCommand([]{}, {}).WithTimeout(.1);

  scheduler.Schedule(&command);

  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));

  std::this_thread::sleep_for(std::chrono::milliseconds(150));

  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(CommandDecoratorTest, InterruptOnTest) {
  CommandScheduler scheduler = GetScheduler();

  bool finished = false;

  auto command = RunCommand([]{}, {}).InterruptOn([&finished]{return finished;});

  scheduler.Schedule(&command);

  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));

  finished = true;
  
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}