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
      m_timer = std::make_unique<frc::Timer>();
    }

    WaitCommand(WaitCommand&& other) = default;
    
    void Initialize() override {
      m_timer->Reset();
      m_timer->Start();
    }
    
    void End(bool interrupted) override {
      m_timer->Stop();
    }
    
    bool IsFinished() override {
      return m_timer->HasPeriodPassed(m_duration);
    }
    
    bool RunsWhenDisabled() const override {
      return true;
    }
 protected:
  protected:
  std::unique_ptr<Command> TransferOwnership()&& override {
    return std::make_unique<WaitCommand>(std::move(*this));
  }
  std::unique_ptr<frc::Timer> m_timer;
 private:
  double m_duration;
};
}
}
