#pragma once

#include "CommandGroupBase.h"
#include <unordered_map>

namespace frc {
namespace experimental {
class ParallelDeadlineGroup : public CommandGroupBase {
 public:
  ParallelDeadlineGroup(std::unique_ptr<Command>&& deadline, std::vector<std::unique_ptr<Command>>&& commands) 
      : m_deadline{std::move(deadline)} {
    AddCommands(std::move(commands));
    m_deadline->SetGrouped(true);
    m_commands[m_deadline] = false;
    AddRequirements(m_deadline->GetRequirements());
    m_runWhenDisabled &= m_deadline->RunsWhenDisabled();
  }

  //TODO: add copy constructor that makes deep copy?
  ParallelDeadlineGroup(const ParallelDeadlineGroup&) = delete;
  
  void SetDeadline(std::unique_ptr<Command>&& deadline) {
    m_deadline = std::move(deadline);
    m_deadline->SetGrouped(true);
    m_commands[m_deadline] = false;
    AddRequirements(m_deadline->GetRequirements());
    m_runWhenDisabled &= m_deadline->RunsWhenDisabled();
  }
  
  void AddCommands(std::vector<std::unique_ptr<Command>>&& commands) override {
    if (!RequireUngrouped(commands)) {
      return;
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
    for (auto& commandRunning : m_commands) {
      if (commandRunning.second) {
        commandRunning.first->End(true);
      }
    }
  }
  
  bool IsFinished() override {
    return m_deadline->IsFinished();
  }
  
  bool RunsWhenDisabled() const override {
    return m_runWhenDisabled;
  }
  
 private:
  std::unordered_map<std::unique_ptr<Command>, bool> m_commands;
  std::unique_ptr<Command> m_deadline;
  bool m_runWhenDisabled{true};
};
}
}
