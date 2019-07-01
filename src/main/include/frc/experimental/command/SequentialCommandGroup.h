#pragma once

#include "CommandGroupBase.h"
#include <wpi/ArrayRef.h>
#include "frc/WPIErrors.h"
#include "frc/ErrorBase.h"

namespace frc {
namespace experimental {
class SequentialCommandGroup : public CommandGroupBase, public ErrorBase {
 public:
  SequentialCommandGroup(std::vector<std::unique_ptr<Command>>&& commands) {
    AddCommands(std::move(commands));
  }

  SequentialCommandGroup(SequentialCommandGroup&& other) = default;
  
  //TODO: add copy constructor that makes deep copy?
  SequentialCommandGroup(const SequentialCommandGroup&) = delete;
 
  void AddCommands(std::vector<std::unique_ptr<Command>>&& commands) final {
    if (!RequireUngrouped(commands)) {
      return;
    }
    
    if (m_currentCommandIndex != -1) {
      wpi_setWPIErrorWithContext(CommandIllegalUse, 
          "Commands cannot be added to a CommandGroup while the group is running");
    }
        
    for(auto&& command : commands) {
      command->SetGrouped(true);
      AddRequirements(command->GetRequirements());
      m_runWhenDisabled &= command->RunsWhenDisabled();
      m_commands.emplace_back(std::move(command));
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
    
    auto& currentCommand = m_commands[m_currentCommandIndex];
    
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
    if (interrupted && !m_commands.empty()) {
      m_commands[m_currentCommandIndex]->End(interrupted);
    }
    m_currentCommandIndex = -1;
  }
  
  bool IsFinished() override {
    return m_currentCommandIndex == m_commands.size();
  }
  
  bool RunsWhenDisabled() const override {
    return m_runWhenDisabled;
  }
 protected:
  std::unique_ptr<Command> TransferOwnership()&& override {
    return std::make_unique<SequentialCommandGroup>(std::move(*this));
  } 
 private:
  wpi::SmallVector<std::unique_ptr<Command>, 4> m_commands;
  int m_currentCommandIndex{-1};
  bool m_runWhenDisabled{true};
};
}
}
