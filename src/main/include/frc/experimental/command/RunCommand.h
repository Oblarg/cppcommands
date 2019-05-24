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
    
  void Execute() override {
    m_toRun();
  }
 protected:
  std::function<void()> m_toRun;
};
}
}
