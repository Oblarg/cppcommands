#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "frc/experimental/command/CommandScheduler.h"
#include "frc/experimental/command/CommandGroupBase.h"
#include "frc/experimental/command/SendableSubsystemBase.h"
#include "frc/experimental/command/SendableSubsystemBase.h"
#include "frc/experimental/command/SetUtilities.h"

namespace frc {
namespace experimental {
class CommandTestBase : public ::testing::Test {
 public:
  CommandTestBase() {
    auto& scheduler = CommandScheduler::GetInstance();
    scheduler.CancelAll();
    scheduler.Enable();
    scheduler.ClearButtons();
    CommandGroupBase::ClearGroupedCommands();
  }
  
  class TestSubsystem : public SendableSubsystemBase {
    
  };
  
  class MockCommandHolder {
   public:
    class MockCommand : public Command {
     public:
      MOCK_CONST_METHOD0(GetRequirements, wpi::ArrayRef<Subsystem*>());
      MOCK_METHOD0(IsFinished, bool());
      MOCK_METHOD0(RunsWhenDisabled, bool());
    };
    MockCommandHolder(bool runWhenDisabled, wpi::ArrayRef<Subsystem*> requirements) {
      m_requirements = requirements;
      EXPECT_CALL(m_mockCommand, GetRequirements()).WillRepeatedly(::testing::Return(m_requirements));
      EXPECT_CALL(m_mockCommand, IsFinished()).WillRepeatedly(::testing::Return(false));
      EXPECT_CALL(m_mockCommand, RunsWhenDisabled).WillRepeatedly(::testing::Return(runWhenDisabled));
    }
    
    Command* GetMock() {
      return &m_mockCommand;
    }
    
    void SetFinished(bool finished) {
      EXPECT_CALL(m_mockCommand, IsFinished()).WillRepeatedly(::testing::Return(finished));
    }
   private:
    std::vector<Subsystem*> m_requirements;
    MockCommand m_mockCommand;
  };
};
}
}
