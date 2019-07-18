#pragma once

#include "gtest/gtest.h"
#include "make_vector.h"
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

 protected:

  class MockCommand : public Command {
     public:
      MOCK_CONST_METHOD0(GetRequirements, wpi::SmallSet<Subsystem*, 4>());
      MOCK_METHOD0(IsFinished, bool());
      MOCK_CONST_METHOD0(RunsWhenDisabled, bool());
      MOCK_METHOD0(Initialize, void());
      MOCK_METHOD0(Execute, void());
      MOCK_METHOD1(End, void(bool interrupted));

      MockCommand() {
        m_requirements = {};
        EXPECT_CALL(*this, GetRequirements()).WillRepeatedly(::testing::Return(m_requirements));
        EXPECT_CALL(*this, IsFinished()).WillRepeatedly(::testing::Return(false));
        EXPECT_CALL(*this, RunsWhenDisabled()).WillRepeatedly(::testing::Return(true));
      };

      MockCommand(wpi::ArrayRef<Subsystem*> requirements, bool finished = false, bool runWhenDisabled = true) {
        m_requirements.insert(requirements.begin(), requirements.end());
        EXPECT_CALL(*this, GetRequirements()).WillRepeatedly(::testing::Return(m_requirements));
        EXPECT_CALL(*this, IsFinished()).WillRepeatedly(::testing::Return(finished));
        EXPECT_CALL(*this, RunsWhenDisabled()).WillRepeatedly(::testing::Return(runWhenDisabled));
      };

      MockCommand(MockCommand&& other)  {
        EXPECT_CALL(*this, GetRequirements()).WillRepeatedly(::testing::Return(other.m_requirements));
        EXPECT_CALL(*this, IsFinished()).WillRepeatedly(::testing::Return(other.IsFinished()));
        EXPECT_CALL(*this, RunsWhenDisabled()).WillRepeatedly(::testing::Return(other.RunsWhenDisabled()));
        std::swap(m_requirements, other.m_requirements);
      };

      MockCommand(const MockCommand& other) {};

      void SetFinished(bool finished) {
      EXPECT_CALL(*this, IsFinished()).WillRepeatedly(::testing::Return(finished));
      }

      ~MockCommand() {
        auto& scheduler = CommandScheduler::GetInstance();
        scheduler.Cancel(this);
      }
     protected:
      std::unique_ptr<Command> TransferOwnership()&& {
        return std::make_unique<MockCommand>(std::move(*this));
      }
     private:
      wpi::SmallSet<Subsystem*, 4> m_requirements;
    };

  CommandScheduler GetScheduler();

  virtual void SetUp();

  virtual void TearDown();

  void SetDSEnabled(bool enabled);
};
}
}
