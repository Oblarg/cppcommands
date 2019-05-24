#pragma once

#include "SendableCommandBase.h"
#include <frc/Notifier.h>

namespace frc {
namespace experimental {
class NotifierCommand : public SendableCommandBase {
 public:
  NotifierCommand(std::function<void()> toRun, double period, wpi::ArrayRef<Subsystem*> requirements) :
    m_notifier{std::move(toRun)}, m_period{period} {
      AddRequirements(requirements);
  }
  
  void Initialize() override {
    m_notifier.StartPeriodic(m_period);
  }
  
  void End(bool interrupted) override {
    m_notifier.Stop();
  }
 private:
  Notifier m_notifier;
  double m_period;
};
}
}
