#pragma once

#include "CommandGroupBase.h"
#include <unordered_map>

namespace frc {
namespace experimental {
class ParallelCommandGroup : public CommandGroupBase {
 public:
  ParallelCommandGroup(std::vector<std::unique_ptr<Command>>&& commands) {
    AddCommands(std::move(commands));
  }

  ParallelCommandGroup(ParallelCommandGroup&& other) = default;

  //TODO: add copy constructor that makes a deep copy?
  ParallelCommandGroup(const ParallelCommandGroup&) = delete;

  void AddCommands(std::vector<std::unique_ptr<Command>>&& commands) override {
    for (auto&& command : commands) {
      if (!RequireUngrouped(*command)) return;
    }
    
    // TODO: Running Group
    
    // TODO: Disjoint
    for(auto&& command : commands) {
      command->SetGrouped(true);
      AddRequirements(command->GetRequirements());
      m_runWhenDisabled &= command->RunsWhenDisabled();
      m_commands[std::move(command)] = false;
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
  
  bool RunsWhenDisabled() const override {
    return m_runWhenDisabled;
  }
 protected:
  std::unique_ptr<Command> TransferOwnership()&& override {
    return std::make_unique<ParallelCommandGroup>(std::move(*this));
  } 
 private:
  std::unordered_map<std::unique_ptr<Command>, bool> m_commands;
  bool m_runWhenDisabled{true};
};
}
}
