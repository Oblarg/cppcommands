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
  template <class T, typename = std::enable_if_t<std::is_base_of<Command, T>::value>>
  SelectCommand(std::vector<std::pair<Key, T&&>> commands, std::function<Key()> selector) 
    : m_selector{std::move(selector)} {
    for (auto&& command : commands) {
      if (!CommandGroupBase::RequireUngrouped(command.second)) {
        return;
      }
    }

    for (auto&& command : commands) {
      AddRequirements(command.second->GetRequirements());
      m_commands.emplace(std::move(command.first), std::make_unique<T>(std::forward<T>(command.second)));
    }
  }


  // SelectCommand(std::initializer_list<std::pair<Key, std::unique_ptr<Command>>>&& commands, std::function<Key()> selector) 
  //   : m_selector{std::move(selector)} {
  //   for (auto&& command : commands) {
  //     if (!CommandGroupBase::RequireUngrouped(command.second)) {
  //       return;
  //     }
  //   }

  //   for (auto&& command : commands) {
  //     AddRequirements(command.second->GetRequirements());
  //     m_commands.emplace(std::move(command.first), std::move(command.second));
  //   }
  // }
  
  explicit SelectCommand(std::function<Command*()> toRun) 
    : m_toRun{toRun} {
  }

  SelectCommand(SelectCommand&& other) = default;
  
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
  
  bool RunsWhenDisabled() const override {
    return m_selectedCommand->RunsWhenDisabled();
  }
 protected:
  std::unique_ptr<Command> TransferOwnership()&& override {
    return std::make_unique<SelectCommand>(std::move(*this));
  }
 private:
  std::unordered_map<Key, std::unique_ptr<Command>> m_commands;
  std::function<Key()> m_selector;
  std::function<std::unique_ptr<Command>()> m_toRun;
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
    m_selectedCommand = find->second.get();
  } else {
    m_selectedCommand = m_toRun().get();
  }
  m_selectedCommand->Initialize();
}

}
}
