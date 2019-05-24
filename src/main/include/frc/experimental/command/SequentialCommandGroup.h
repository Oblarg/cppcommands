#pragma once

#include "CommandGroupBase.h"
#include <wpi/ArrayRef.h>

namespace frc {
namespace experimental {
class SequentialCommandGroup : public CommandGroupBase {
 public:
  SequentialCommandGroup(wpi::ArrayRef<Command*> commands) {
    AddCommands(commands);
  }
 
  void AddCommands(wpi::ArrayRef<Command*> commands) final {
    RequireUngrouped(commands);
    
    if (m_currentCommandIndex != -1) {
      // Error
    }
    
    RegisterGroupedCommands(commands);
    
    for(auto command : commands) {
      m_commands.emplace_back(command);
      AddRequirements(command->GetRequirements());
      m_runWhenDisabled &= command->RunsWhenDisabled();
    }
  }
  
  void Initialize() override {
    m_currentCommandIndex = 0;
    
    if (!m_commands.empty()) {
      m_commands[0]->Initialize();
    }
  }
  
  void Execute() override {
    if (m_commands.empty()) return;
    
    auto currentCommand = m_commands[m_currentCommandIndex];
    
    currentCommand->Execute();
    if (currentCommand->IsFinished()) {
      currentCommand->End(false);
      m_currentCommandIndex++;
      if (m_currentCommandIndex < m_commands.size()) {
        m_commands[m_currentCommandIndex]->Initialize();
      }
    }
  }
  
  void End(bool interrupted) override {
    if (interrupted && ! m_commands.empty()) {
      m_commands[m_currentCommandIndex]->End(interrupted);
    }
    m_currentCommandIndex = -1;
  }
  
  bool IsFinished() override {
    return m_currentCommandIndex == m_commands.size();
  }
  
  bool RunsWhenDisabled() override {
    return m_runWhenDisabled;
  }
  
 private:
  wpi::SmallVector<Command*, 4> m_commands;
  int m_currentCommandIndex{-1};
  bool m_runWhenDisabled{true};
};
}
}
