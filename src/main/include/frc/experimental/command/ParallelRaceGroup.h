#pragma once

#include "CommandGroupBase.h"
#include <unordered_map>

namespace frc {
namespace experimental {
class ParallelRaceGroup : public CommandGroupBase {
 public:
  ParallelRaceGroup(std::initializer_list<Command*> commands) {
    AddCommands(commands);
  }
  
  void AddCommands(std::initializer_list<Command*> commands) override {
    RequireUngrouped(commands);
    
    // TODO: Running Group
    
    RegisterGroupedCommands(commands);
    
    // TODO: Disjoint
    for(auto command : commands) {
      m_commands.emplace(command);
      auto& requirements = command->GetRequirements();
      AddRequirements(requirements.begin(), requirements.end());
      m_runWhenDisabled &= command->RunsWhenDisabled();
    }
  }
  
  void Initialize() override {
    for (auto& commandRunning : m_commands) {
      commandRunning->Initialize();
    }
  }
  
    void Execute() override {
    for (auto& commandRunning : m_commands) {
      commandRunning->Execute();
      if (commandRunning->IsFinished()) {
        m_finished = true;
        commandRunning->End(false);
      }
    }
  }
  
  void End(bool interrupted) override {
    for (auto& commandRunning : m_commands) {
      if (commandRunning->IsFinished()) {
        commandRunning->End(true);
      }
    }
  }
  
  bool IsFinished() override {
    return m_finished;
  }
  
  bool RunsWhenDisabled() override {
    return m_runWhenDisabled;
  }
  
 private:
  std::set<Command*> m_commands;
  bool m_runWhenDisabled{true};
  bool m_finished{false};
};
}
}
