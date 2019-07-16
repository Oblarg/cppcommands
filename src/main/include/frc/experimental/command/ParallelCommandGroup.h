#pragma once

#include "CommandGroupBase.h"
#include "CommandHelper.h"
#include <unordered_map>

namespace frc {
namespace experimental {
class ParallelCommandGroup : public CommandHelper<CommandGroupBase, ParallelCommandGroup> {
 public:
  ParallelCommandGroup(std::vector<std::unique_ptr<Command>>&& commands) {
    AddCommands(std::move(commands));
  }

  template <class... Types, typename = std::enable_if_t<std::conjunction_v<std::is_base_of<Command, Types>...>>>
  ParallelCommandGroup(Types&&... commands) {
    AddCommands(std::forward<Types>(commands)...);
  }

  ParallelCommandGroup(ParallelCommandGroup&& other) = default;

  //TODO: add copy constructor that makes a deep copy?
  ParallelCommandGroup(const ParallelCommandGroup&) = delete;

  template <class... Types>
  void AddCommands(Types&&... commands) {
    std::vector<std::unique_ptr<Command>> foo;
    ((void)foo.emplace_back(std::make_unique<Types>(std::forward<Types>(commands))), ...);
    AddCommands(std::move(foo));
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
 private:
  void AddCommands(std::vector<std::unique_ptr<Command>>&& commands) override {
    for (auto&& command : commands) {
      if (!RequireUngrouped(*command)) return;
    }
    
    // TODO: Running Group
    
    // TODO: Disjoint
    for(auto&& command : commands) {
      if(RequirementsDisjoint(this, command.get())) {
        command->SetGrouped(true);
        AddRequirements(command->GetRequirements());
        m_runWhenDisabled &= command->RunsWhenDisabled();
        m_commands[std::move(command)] = false;
      }
      else {
        wpi_setWPIErrorWithContext(CommandIllegalUse, 
          "Multiple commands in a parallel group cannot require the same subsystems");
        return;
      }
    }
  }

  std::unordered_map<std::unique_ptr<Command>, bool> m_commands;
  bool m_runWhenDisabled{true};
};
}
}
