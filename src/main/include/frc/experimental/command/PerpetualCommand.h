#pragma once

#include "SendableCommandBase.h"
#include "CommandGroupBase.h"

namespace frc {
namespace experimental {
class PerpetualCommand : public SendableCommandBase {
 public:
  explicit PerpetualCommand(Command* command) 
    : m_command{command} {
      CommandGroupBase::RequireUngrouped({command});
      CommandGroupBase::RegisterGroupedCommands({command});
      AddRequirements(command->GetRequirements());
    }
    
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
  Command* m_command;
};
}
}
