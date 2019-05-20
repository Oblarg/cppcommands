#pragma once

#include "SendableCommandBase.h"

namespace frc {
namespace experimental {
class ScheduleCommand : public SendableCommandBase {
 public:
  explicit ScheduleCommand(std::initializer_list<Command*> toSchedule) {
    m_toSchedule.insert(toSchedule);
  }
  
  void Initialize() override {
    for (auto command : m_toSchedule) {
      command->Schedule();
    }
  }
  
  bool IsFinished() override {
    return true;
  }
  
  bool RunsWhenDisabled() override {
    return true;
  }
 private:
  std::set<Command*> m_toSchedule;
};
}
}
