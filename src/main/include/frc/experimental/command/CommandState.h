#pragma once

#include <atomic>

namespace frc {
namespace experimental {
class CommandState final {
 public:
  explicit CommandState(bool interruptible) : m_interruptible{interruptible} {
    StartTiming();
    StartRunning();
  }

  void StartTiming();
  void StartRunning();
  bool IsInterruptible() const { return m_interruptible; }
  double TimeSinceInitialized();
 private:
  std::atomic<double> m_startTime;
  const bool m_interruptible;
};
}
}
