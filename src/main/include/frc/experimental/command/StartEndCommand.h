#pragma once

#include "SendableCommandBase.h"

namespace frc {
namespace experimental {
class StartEndCommand : public SendableCommandBase {
 public:
  StartEndCommand(std::function<void()> onInit, std::function<void()> onEnd, wpi::ArrayRef<Subsystem*> requirements) 
    : m_onInit{std::move(onInit)}, m_onEnd{std::move(onEnd)} {
    AddRequirements(requirements);
  }
  
  void Initialize() override {
    m_onInit();
  }
  
  void End(bool interrupted) override {
    m_onEnd();
  }
 protected:
  std::function<void()> m_onInit;
  std::function<void()> m_onEnd;
};
}
}
