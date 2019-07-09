#pragma once

#include "SendableCommandBase.h"
#include "CommandHelper.h"

namespace frc {
namespace experimental {
class PerpetualCommand : public CommandHelper<SendableCommandBase, PerpetualCommand> {
 public:
  explicit PerpetualCommand(std::unique_ptr<Command> command) {
      if (!CommandGroupBase::RequireUngrouped(command)) { 
        return; 
      }
      m_command = std::move(command);
      m_command->SetGrouped(true);
      AddRequirements(m_command->GetRequirements());
    }

  PerpetualCommand(PerpetualCommand&& other) = default;
    
  void Initialize() override {
    m_command->Initialize();
  }
  
  void Execute() override {
    m_command->Execute();
  }
  
  void End(bool interrupted) override {
    m_command->End(interrupted);
  }
 private:
  std::unique_ptr<Command> m_command;
};
}
}
