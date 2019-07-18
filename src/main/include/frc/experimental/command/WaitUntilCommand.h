#pragma once

#include "SendableCommandBase.h"
#include "frc/experimental/command/CommandHelper.h"
#include "frc/Timer.h"

namespace frc {
namespace experimental {
class WaitUntilCommand : public CommandHelper<SendableCommandBase, WaitUntilCommand> {
 public:
    WaitUntilCommand(std::function<bool()> condition) 
    : m_condition{std::move(condition)} {
    }
    
    WaitUntilCommand(double time) 
      : m_condition{[=]{ return Timer::GetMatchTime() - time > 0; }} {
    }
    
    WaitUntilCommand(WaitUntilCommand&& other) = default;

    WaitUntilCommand(const WaitUntilCommand& other) {
      m_condition = other.m_condition;
      m_name = other.m_name;
      m_requirements = other.m_requirements;
      m_subsystem = other.m_subsystem;
    };
    
    bool IsFinished() override {
      return m_condition();
    }
    
    bool RunsWhenDisabled() const override {
      return true;
    }
 private:
  std::function<bool()> m_condition;
};
}
}
