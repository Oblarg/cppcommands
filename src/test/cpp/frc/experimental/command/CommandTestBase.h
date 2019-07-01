#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "frc/experimental/command/CommandScheduler.h"
#include "frc/experimental/command/CommandGroupBase.h"
#include "simulation/DriverStationSim.h"
#include "frc/experimental/command/SendableSubsystemBase.h"
#include "frc/experimental/command/SendableSubsystemBase.h"
#include "frc/experimental/command/SetUtilities.h"
#include "ErrorConfirmer.h"
#include <regex>
#include "mockdata/MockHooks.h"

namespace frc {
namespace experimental {
class CommandTestBase : public ::testing::Test {
 public:
  CommandTestBase();
  
  class TestSubsystem : public SendableSubsystemBase {
    
  };
  
  class MockCommandHolder {
   public:
    class MockCommand : public Command {
     public:
      MOCK_CONST_METHOD0(GetRequirements, wpi::ArrayRef<Subsystem*>());
      MOCK_METHOD0(IsFinished, bool());
      MOCK_METHOD0(RunsWhenDisabled, bool());
      MOCK_METHOD0(Initialize, void());
      MOCK_METHOD0(Execute, void());
      MOCK_METHOD1(End, void(bool interrupted));

      ~MockCommand() {
        auto& scheduler = CommandScheduler::GetInstance();
        scheduler.Cancel(this);
      }
    };
    MockCommandHolder(bool runWhenDisabled, wpi::ArrayRef<Subsystem*> requirements) {
      m_requirements = requirements;
      EXPECT_CALL(*m_mockCommand, GetRequirements()).WillRepeatedly(::testing::Return(m_requirements));
      EXPECT_CALL(*m_mockCommand, IsFinished()).WillRepeatedly(::testing::Return(false));
      EXPECT_CALL(*m_mockCommand, RunsWhenDisabled).WillRepeatedly(::testing::Return(runWhenDisabled));
    }

    
    std::unique_ptr<MockCommand>&& GetMock() {
      return std::move(m_mockCommand);
    }
    
    void SetFinished(bool finished) {
      EXPECT_CALL(m_mockCommand, IsFinished()).WillRepeatedly(::testing::Return(finished));
    }
   private:
    std::vector<Subsystem*> m_requirements;
    std::unique_ptr<MockCommand> m_mockCommand;
  };

 protected:

   CommandScheduler GetScheduler();

   virtual void SetUp();

   virtual void TearDown();
};
}
}
