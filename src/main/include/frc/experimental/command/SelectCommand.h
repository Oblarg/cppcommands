#pragma once

#include <initializer_list>
#include <unordered_map>

#include "SendableCommandBase.h"
#include "CommandGroupBase.h"
#include "PrintCommand.h"

namespace frc {
namespace experimental {
template <typename Key>
class SelectCommand : public SendableCommandBase {
 public:
  SelectCommand(std::initializer_list<std::pair<Key, Command*>> commands, std::function<Key()> selector) 
    : m_selector{std::move(selector)} {
    for (auto& command : commands) {
      if (!CommandGroupBase::RequireUngrouped({command.second})) {
        return;
      }
    }
    m_commands = {commands.begin(), commands.end()};
    for (auto& command : commands) {
      AddRequirements(command.second->GetRequirements());
    }
  }
  
  explicit SelectCommand(std::function<Command*()> toRun) 
    : m_toRun{toRun} {
  }
  
  void Initialize() override;
  
  void Execute() override {
    m_selectedCommand->Execute();
  }
  
  void End(bool interrupted) override {
    return m_selectedCommand->End(interrupted);
  }
  
  bool IsFinished() override {
    return m_selectedCommand->IsFinished();
  }
  
  bool RunsWhenDisabled() override {
    return m_selectedCommand->RunsWhenDisabled();
  }
 private:
  std::unordered_map<Key, Command*> m_commands;
  std::function<Key()> m_selector;
  std::function<Command*()> m_toRun;
  Command* m_selectedCommand;
};

template <typename T>
void SelectCommand<T>::Initialize() {
  if (m_selector) {
    auto find = m_commands.find(m_selector());
    if (find == m_commands.end()) {
      m_selectedCommand = new PrintCommand("SelectCommand selector value does not correspond to any command!");
      return;
    }
    m_selectedCommand = find->second;
  } else {
    m_selectedCommand = m_toRun();
  }
  m_selectedCommand->Initialize();
}

}
}
