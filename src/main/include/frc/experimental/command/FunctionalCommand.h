#pragma once

#include "SendableCommandBase.h"
#include "CommandGroupBase.h"

namespace frc {
namespace experimental {
class FunctionalCommand : public SendableCommandBase {
 public:
  FunctionalCommand(std::function<void()> onInit, std::function<void()> onExecute, std::function<void(bool)> onEnd, std::function<bool()> isFinished)
    : m_onInit{std::move(onInit)}, m_onExecute{std::move(onExecute)}, m_onEnd{std::move(onEnd)}, m_isFinished{std::move(isFinished)} {
    }

    FunctionalCommand(FunctionalCommand&& other) = default;

    // FunctionalCommand(FunctionalCommand&& other)
    //   : SendableCommandBase(std::move(other)),
    //   m_onInit(std::move(other.m_onInit)),
    //   m_onExecute(std::move(other.m_onExecute)),
    //   m_onEnd(std::move(other.m_onEnd)),
    //   m_isFinished(std::move(other.m_isFinished)) {
    //     m_isGrouped = other.m_isGrouped;
    //   }
    
    void Initialize() override {
      m_onInit();
    }
    
    void Execute() override {
      m_onExecute();
    }
    
    void End(bool interrupted) override {
      m_onEnd(interrupted);
    }
    
    bool IsFinished() override {
      return m_isFinished();
    }
 protected:
  std::unique_ptr<Command> TransferOwnership()&& override {
    return std::make_unique<FunctionalCommand>(std::move(*this));
  }
 private:
  std::function<void()> m_onInit;
  std::function<void()> m_onExecute;
  std::function<void(bool)> m_onEnd;
  std::function<bool()> m_isFinished;
};
}
}
