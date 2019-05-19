#pragma once

#include "CommandGroupBase.h"
#include <unordered_map>

namespace frc {
namespace experimental {
class ParallelCommandGroup : public CommandGroupBase {
 public:
  ParallelCommandGroup(std::initializer_list<Command*> commands) {
    AddCommands(commands);
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
    for (auto& command : m_commands) {
      if (command.second) return false;
    }
    return true;
  }
  
  bool RunsWhenDisabled() override {
    return m_runWhenDisabled;
  }
  
 private:
  std::unordered_map<Command*, bool> m_commands;
  bool m_runWhenDisabled{true};
};
}
}
