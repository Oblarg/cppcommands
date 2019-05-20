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
 protected:
  frc::Timer m_timer;
 private:
  double m_duration;
};
}
}
