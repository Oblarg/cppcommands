#pragma once

#include "SendableCommandBase.h"
#include "frc/Timer.h"

namespace frc {
namespace experimental {
class WaitUntilCommand : public SendableCommandBase {
 public:
  explicit WaitUntilCommand(std::function<bool()> condition) 
    : m_condition{std::move(condition)} {
    }
    
    explicit WaitUntilCommand(double time) 
      : m_condition{[=]{ return Timer::GetMatchTime() - time > 0; }} {
    }
    
    
    bool IsFinished() override {
      return m_condition();
    }
    
    bool RunsWhenDisabled() override {
      return true;
    }
 private:
  std::function<bool()> m_condition;
};
}
}
