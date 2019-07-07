#pragma once

#include "CommandGroupBase.h"
#include <unordered_map>

namespace frc {
namespace experimental {
class ParallelRaceGroup : public CommandGroupBase {
 public:
  ParallelRaceGroup(std::vector<std::unique_ptr<Command>>&& commands) {
    AddCommands(std::move(commands));
  }

  template <class... Types>
  ParallelRaceGroup(Types&&... commands) {
    std::vector<std::unique_ptr<Command>> foo;
    ((void)foo.emplace_back(std::make_unique<Types>(std::forward<Types>(commands))), ...);
    AddCommands(std::move(foo));
  }

  ParallelRaceGroup(ParallelRaceGroup&& other) = default;

  //TODO: add copy constructor that makes deep copy?
  ParallelRaceGroup(const ParallelRaceGroup&) = delete;
  
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
      m_commands.emplace(std::move(command));
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
      if (!commandRunning->IsFinished()) {
        commandRunning->End(true);
      }
    }
  }
  
  bool IsFinished() override {
    return m_finished;
  }
  
  bool RunsWhenDisabled() const override {
    return m_runWhenDisabled;
  }
 protected:
  std::unique_ptr<Command> TransferOwnership()&& override {
    return std::make_unique<ParallelRaceGroup>(std::move(*this));
  } 
 private:
  std::set<std::unique_ptr<Command>> m_commands;
  bool m_runWhenDisabled{true};
  bool m_finished{false};
};
}
}
