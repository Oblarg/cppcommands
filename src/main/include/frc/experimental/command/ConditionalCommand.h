#pragma once

#include "SendableCommandBase.h"
#include "CommandGroupBase.h"
#include <iostream>

namespace frc {
namespace experimental {
class ConditionalCommand : public SendableCommandBase {
 public:
  template <class T1, class T2, 
    typename = std::enable_if_t<std::is_base_of<Command, T1>::value>,
    typename = std::enable_if_t<std::is_base_of<Command, T2>::value>>
  ConditionalCommand(T1&& onTrue, T2&& onFalse, std::function<bool()> condition)
    : ConditionalCommand(std::make_unique<T1>(std::forward<T1>(onTrue)),
      std::make_unique<T2>(std::forward<T2>(onFalse)),
      condition) {}

    ConditionalCommand(ConditionalCommand&& other) = default;

    // ConditionalCommand(ConditionalCommand&& other) 
    //   : SendableCommandBase(std::move(other)),
    //   m_onTrue(std::move(other.m_onTrue)), 
    //   m_onFalse(std::move(other.m_onFalse)),
    //   m_condition(std::move(other.m_condition)) {
    //     m_isGrouped = other.m_isGrouped;
    // }
    
    void Initialize() override {
      if (m_condition()) {
        m_selectedCommand = m_onTrue.get();
      } else {
        m_selectedCommand = m_onFalse.get();
      }
      m_selectedCommand->Initialize();
    }
    
    void Execute() override {
      m_selectedCommand->Execute();
    }
    
    void End(bool interrupted) override {
      m_selectedCommand->End(interrupted);
    }
    
    bool IsFinished() override {
      return m_selectedCommand->IsFinished();
    }
    
    bool RunsWhenDisabled() const override {
      return m_selectedCommand->RunsWhenDisabled();
    }
 protected:
  std::unique_ptr<Command> TransferOwnership()&& override {
    return std::make_unique<ConditionalCommand>(std::move(*this));
  }
 private:
  ConditionalCommand(std::unique_ptr<Command>&& onTrue, std::unique_ptr<Command>&& onFalse, std::function<bool()> condition)
    : m_condition{std::move(condition)} {
      if (!CommandGroupBase::RequireUngrouped({onTrue.get(), onFalse.get()})) {
        return;
      }

      m_onTrue = std::move(onTrue);
      m_onFalse = std::move(onFalse);

      m_onTrue->SetGrouped(true);
      m_onFalse->SetGrouped(true);

      AddRequirements(m_onTrue->GetRequirements());
      AddRequirements(m_onFalse->GetRequirements());
  }
  std::unique_ptr<Command> m_onTrue;
  std::unique_ptr<Command> m_onFalse;
  std::function<bool()> m_condition;
  Command* m_selectedCommand{nullptr};
};
}
}
