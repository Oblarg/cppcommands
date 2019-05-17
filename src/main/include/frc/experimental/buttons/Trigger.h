#pragma once

#include <atomic>
#include <frc/smartdashboard/SendableBase.h>

namespace frc {
namespace experimental {
class Command;
class Trigger : public frc::SendableBase {
 public:
  explicit Trigger(std::function<bool()> isActive) : m_isActive{std::move(isActive)} {
  }

  Trigger() {
    m_isActive = []{ return false; };
  }

  virtual bool Get() const {
    return m_isActive();
  }

  virtual bool Grab() const {
    return Get() || m_sendablePressed.load();
  }

  Trigger* WhenActive(Command* command, bool interruptible);
  Trigger* WhenActive(Command* command) { WhenActive(command, true); return this; }
  Trigger* WhenActive(std::function<void()> toRun);
  Trigger* WhileActiveContinous(Command* command, bool interruptible);
  Trigger* WhileActiveContinous(Command* command) {
    WhileActiveContinous(command, true);
    return this;
  }
  Trigger* WhileActiveContinous(std::function<void()> toRun);
  Trigger* WhileActiveOnce(Command* command, bool interruptible);
  Trigger* WhileActiveOnce(Command* command) { WhileActiveOnce(command, true); return this; }
  Trigger* WhenInactive(Command* command, bool interruptible);
  Trigger* WhenInactive(Command* command) { WhenInactive(command, true); return this; }
  Trigger* WhenInactive(std::function<void()> toRun);
  Trigger* ToggleWhenActive(Command* command, bool interruptible);
  Trigger* ToggleWhenActive(Command* command) { ToggleWhenActive(command, true); return this; }
  Trigger* CancelWhenActive(Command* command);

  Trigger* And(Trigger* trigger) {
    return new Trigger([this, trigger]{ return Grab() && trigger->Grab(); });
  }

  void InitSendable(frc::SendableBuilder& builder) override;
 private:
  std::atomic_bool m_sendablePressed{false};
  std::function<bool()> m_isActive;
};
}
}
