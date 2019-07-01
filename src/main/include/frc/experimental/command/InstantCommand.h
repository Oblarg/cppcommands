#pragma once

#include "SendableCommandBase.h"

namespace frc {
namespace experimental {
class InstantCommand : public SendableCommandBase {
 public:
  InstantCommand(std::function<void()> toRun, wpi::ArrayRef<Subsystem*> requirements) : m_toRun{std::move(toRun)} {
    AddRequirements(requirements);
  }

  InstantCommand(InstantCommand&& other) = default;

  InstantCommand() : m_toRun{[]{}} {
  }

  void Initialize() override {
    m_toRun();
  }

  bool IsFinished() final override {
    return true;
  }
 protected:
  std::unique_ptr<Command> TransferOwnership()&& override {
    return std::make_unique<InstantCommand>(std::move(*this));
  }
 private:
  std::function<void()> m_toRun;
};
}
}
