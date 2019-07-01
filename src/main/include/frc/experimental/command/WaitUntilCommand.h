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
    
    bool RunsWhenDisabled() const override {
      return true;
    }
 protected:
  std::unique_ptr<Command> TransferOwnership()&& override {
    return std::make_unique<WaitUntilCommand>(std::move(*this));
  }
 private:
  std::function<bool()> m_condition;
};
}
}
