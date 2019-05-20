#pragma once

#include "CommandGroupBase.h"
#include <unordered_map>

namespace frc {
namespace experimental {
class ParallelDeadlineGroup : public CommandGroupBase {
 public:
  ParallelDeadlineGroup(Command* deadline, std::initializer_list<Command*> commands) 
      : m_deadline{deadline} {
    AddCommands(commands);
    if (m_commands.find(deadline) == m_commands.end()) {
      AddCommands({deadline});
    }
  }
  
  void SetDeadline(Command* deadline) {
    if (m_commands.find(deadline) == m_commands.end()) {
      AddCommands({deadline});
    }
    m_deadline = deadline;
  }
  
  void AddCommands(std::initializer_list<Command*> commands) override {
    RequireUngrouped(commands);
    
    // TODO: Running Group
    
    RegisterGroupedCommands(commands);
    
    // TODO: Disjoint
    for(auto command : commands) {
      m_commands[command] = false;
      auto& requirements = command->GetRequirements();
      AddRequirements(requirements.begin(), requirements.end());
      m_runWhenDisabled &= command->RunsWhenDisabled();
    }
  }
  
  void Initialize() override {
    for (auto& commandRunning : m_commands) {
      commandRunning.first->Initialize();
      commandRunning.second = true;
    }
  }
  
    void Execute() override {
    for (auto& commandRunning : m_commands) {
      if (!commandRunning.second) continue;
      commandRunning.first->Execute();
      if (commandRunning.first->IsFinished()) {
        commandRunning.first->End(false);
        commandRunning.second = false;
      }
    }
  }
  
  void End(bool interrupted) override {
    if (interrupted) {
      for (auto& commandRunning : m_commands) {
        if (commandRunning.second) {
          commandRunning.first->End(true);
        }
      }
    }
  }
  
  bool IsFinished() override {
    return m_deadline->IsFinished();
  }
  
  bool RunsWhenDisabled() override {
    return m_runWhenDisabled;
  }
  
 private:
  std::unordered_map<Command*, bool> m_commands;
  Command* m_deadline;
  bool m_runWhenDisabled{true};
};
}
}
