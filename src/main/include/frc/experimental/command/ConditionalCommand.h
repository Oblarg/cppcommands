#pragma once

#include "SendableCommandBase.h"
#include "CommandGroupBase.h"

namespace frc {
namespace experimental {
class ConditionalCommand : public SendableCommandBase {
 public:
  ConditionalCommand(Command* onTrue, Command* onFalse, std::function<bool()> condition)
    : m_condition{std::move(condition)} {
      if (!CommandGroupBase::RequireUngrouped({onTrue, onFalse})) {
        return;
      }

      CommandGroupBase::RegisterGroupedCommands({onTrue, onFalse});

      m_onTrue = onTrue;
      m_onFalse = onFalse;
      
      AddRequirements(onTrue->GetRequirements());
      AddRequirements(onFalse->GetRequirements());
    }
    
    void Initialize() override {
      if (m_condition()) {
        m_selectedCommand = m_onTrue;
      } else {
        m_selectedCommand = m_onFalse;
      }
      m_selectedCommand->Initialize();
    }
    
    void Execute() override {
      m_selectedCommand->Execute();
    }
    
    void End(bool interrupted) override {
      m_selectedCommand->End(interrupted);
    }
    
    bool IsFinished() override {
      return m_selectedCommand->IsFinished();
    }
    
    bool RunsWhenDisabled() override {
      return m_selectedCommand->RunsWhenDisabled();
    }
 private:
  Command* m_onTrue;
  Command* m_onFalse;
  std::function<bool()> m_condition;
  Command* m_selectedCommand{nullptr};
};
}
}
