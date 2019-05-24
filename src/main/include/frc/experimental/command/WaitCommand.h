#pragma once

#include "SendableCommandBase.h"
#include "frc/Timer.h"
#include "wpi/Twine.h"

namespace frc {
namespace experimental {
class WaitCommand : public SendableCommandBase {
 public:
  explicit WaitCommand(double seconds) 
    : m_duration{seconds} {
      auto secondsStr = std::to_string(seconds);
      SetName(wpi::Twine(m_name) + ": " + wpi::Twine(secondsStr) + " seconds");
    }
    
    void Initialize() override {
      m_timer.Reset();
      m_timer.Start();
    }
    
    void End(bool interrupted) override {
      m_timer.Stop();
    }
    
    bool IsFinished() override {
      return m_timer.HasPeriodPassed(m_duration);
    }
    
    bool RunsWhenDisabled() override {
      return true;
    }
 protected:
  frc::Timer m_timer;
 private:
  double m_duration;
};
}
}
