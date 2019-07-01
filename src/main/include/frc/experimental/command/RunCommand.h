#pragma once

#include "SendableCommandBase.h"

namespace frc {
namespace experimental {
class RunCommand : public SendableCommandBase {
 public:
  RunCommand(std::function<void()> toRun, wpi::ArrayRef<Subsystem*> requirements) 
    : m_toRun{std::move(toRun)} {
      AddRequirements(requirements);
    }

  RunCommand(RunCommand&& other) = default;
    
  void Execute() override {
    m_toRun();
  }
 protected:
  protected:
  std::unique_ptr<Command> TransferOwnership()&& override {
    return std::make_unique<RunCommand>(std::move(*this));
  }
  std::function<void()> m_toRun;
};
}
}
